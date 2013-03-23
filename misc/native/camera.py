#!/usr/bin/python

"""
Copyright (c) 2006-2013 David Osborn

Permission is granted to use and redistribute this software in source and binary
form, with or without modification, subject to the following conditions:

1. Redistributions in source form must retain the above copyright notice, this
   list of conditions, and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions, and the following disclaimer in the documentation
   and/or other materials provided with the distribution, and in the same place
   and form as other copyright, license, and disclaimer information.

As a special exception, distributions of derivative works in binary form may
include an acknowledgement in place of the above copyright notice, this list of
conditions, and the following disclaimer in the documentation and/or other
materials provided with the distribution, and in the same place and form as
other acknowledgements, similar in substance to the following:

	Portions of this software are based on the work of David Osborn.

This software is provided "as is", without any express or implied warranty. In
no event will the authors be liable for any damages arising out of the use of
this software.
"""

# HACK: relative imports do not work with __main__ in Python 2.5
# http://mail.python.org/pipermail/python-list/2006-October/408945.html
#from __future__ import absolute_import

# support local import from zip
# non-zip local import takes precedence
import sys # path
sys.path.insert(1, 'util.zip')

import os.path # dirname, sep
import sys # exit
from OpenGL.GL import *
import wx, wx.glcanvas
from util.config import prefs
from util.math import DegToRad, Euler, Quaternion, RevToRad, Vector2i, Vector3f
from util.opengl import DrawCamera, DrawCameras, DrawCamerasForSelect, DrawScene, DrawTrack, DrawTrackForSelect, SetCameraMatrix, SetCameraMatrixForSelect
from util.phys import Camera
from util.res import CameraSet, Scene, Track
from util.wx import BasicApplication, BasicCanvas, BasicDialog, BasicWindow, GenId, ShowOpenCameraSetDialog, ShowOpenSceneDialog, ShowSaveCameraSetDialog, ShowSaveChangesDialog, ShowSaveSceneDialog, Timer

################################################################################
# application

class Application(BasicApplication):
	# selection constraints
	selectFaceConstraint     = 1
	deselectFaceConstraint   = 2
	selectCameraConstraint   = 3
	deselectCameraConstraint = 4

	def __init__(self):
		BasicApplication.__init__(self)
		# initialize timer
		self.timer = Timer(self.OnTimer)
		# initialize state
		self.scene = Scene()
		self.scene.modified = False
		self.camera = Camera(Vector3f(4), Euler(DegToRad(45), DegToRad(-45)))
		self.selectedCamera = None
		# initialize window
		self.window = Window(self)
		self.window.canvas.OnRender = self.OnRender

	def Update(self, deltaTime):
		if self.window.active:
			# update camera position
			translation = Vector3f()
			if wx.GetKeyState(ord('A')): translation.x -= 1
			if wx.GetKeyState(ord('D')): translation.x += 1
			if wx.GetKeyState(ord('Q')): translation.y -= 1
			if wx.GetKeyState(ord('E')): translation.y += 1
			if wx.GetKeyState(ord('W')): translation.z -= 1
			if wx.GetKeyState(ord('S')): translation.z += 1
			if any(translation):
				if wx.GetKeyState(wx.WXK_CONTROL): translation *= .25;
				if wx.GetKeyState(wx.WXK_SHIFT): translation *= 4;
				self.camera.position += (
					self.camera.orientation * translation *
					prefs['camera.translation.speed'] * deltaTime)
				self.window.canvas.Refresh()

	def AskSave(self):
		if (self.scene.modified or
			self.scene.cameraSet and self.scene.cameraSet.modified):
			result = ShowSaveChangesDialog(self.window)
			if result: self.Save()
			elif result is None: return False
		return True

	def Exit(self):
		if not self.Destroy(): return
		self.window.Close()

	def Destroy(self, ask = True):
		if ask and not self.AskSave(): return False
		self.timer.Stop()
		return True

	# rendering
	def RenderEdit(self):
		glPushAttrib(GL_VIEWPORT)
		viewport = glGetIntegerv(GL_VIEWPORT)
		glViewport(
			viewport[0],     viewport[1],
			viewport[2] / 2, viewport[3])
		SetCameraMatrix(self.camera)
		# draw track
		if self.scene.track:
			DrawTrack(self.scene.track, (
				[face for i, face in enumerate(self.scene.track.faces)
				if self.selectedCamera in self.scene.cameraSet.trackFaces[i].cameras]
				if self.scene.cameraSet else ()))
		# draw cameras
		if self.scene.cameraSet:
			DrawCameras(self.scene.cameraSet.cameras, (self.selectedCamera,))
		glPopAttrib()
	def RenderView(self):
		glPushAttrib(GL_VIEWPORT)
		viewport = glGetIntegerv(GL_VIEWPORT)
		glViewport(
			viewport[0] + viewport[2] / 2, viewport[1],
			viewport[2] - viewport[2] / 2, viewport[3])
		SetCameraMatrix(self.selectedCamera or self.camera)
		# draw scene
		DrawScene(self.scene)
		# draw track
		if self.scene.track:
			DrawTrack(self.scene.track, (
				[face for i, face in enumerate(self.scene.track.faces)
				if self.selectedCamera in self.scene.cameraSet.trackFaces[i].cameras]
				if self.scene.cameraSet else ()))
		glPopAttrib()

	# selection
	def RenderSelect(self, cursorPosition):
		glPushAttrib(GL_VIEWPORT)
		viewport = glGetIntegerv(GL_VIEWPORT)
		glViewport(
			viewport[0],     viewport[1],
			viewport[2] / 2, viewport[3])
		SetCameraMatrixForSelect(self.camera, cursorPosition)
		# initialize selection mode
		glSelectBuffer(256)
		glRenderMode(GL_SELECT)
		glInitNames()
		# draw track
		if self.scene.track:
			glPushName(0)
			DrawTrackForSelect(self.scene.track)
			glPopName()
		# draw cameras
		if self.scene.cameraSet:
			glPushName(1)
			DrawCamerasForSelect(self.scene.cameraSet.cameras)
			glPopName()
		glPopAttrib()
		glFlush()
		return glRenderMode(GL_RENDER)
	def Select(self, cursorPosition):
		nameStack = self.RenderSelect(cursorPosition)
		if nameStack:
			nearest = (None, 1.)
			for near, far, names in nameStack:
				if near < nearest[1]:
					nearest = (names, near)
			if nearest[0]:
				names = nearest[0]
				if names[0] == 0: return self.scene.track.faces[names[1]]
				if names[0] == 1: return self.scene.cameraSet.cameras[names[1]]
				assert not 'invalid name in stack'
		return None
	def UpdateSelection(self, cursorPosition, reset = False):
		if reset: self.__selectConstraint = None
		cursorPosition = Vector2i(cursorPosition)
		if cursorPosition.x < self.window.canvas.Size[0] / 2:
			selection = self.Select(cursorPosition)
			if isinstance(selection, Track.Face):
				if self.selectedCamera:
					i = self.scene.track.faces.index(selection)
					cameras = self.scene.cameraSet.trackFaces[i].cameras
					if self.selectedCamera not in cameras:
						if (not self.__selectConstraint or
							self.__selectConstraint is self.selectFaceConstraint):
							self.__selectConstraint = self.selectFaceConstraint
							cameras.append(self.selectedCamera)
							self.scene.cameraSet.modified = True
							self.window.canvas.Refresh()
					else:
						if (not self.__selectConstraint or
							self.__selectConstraint is self.deselectFaceConstraint):
							self.__selectConstraint = self.deselectFaceConstraint
							cameras.remove(self.selectedCamera)
							self.scene.cameraSet.modified = True
							self.window.canvas.Refresh()
			elif isinstance(selection, CameraSet.Camera):
				if (not self.__selectConstraint or
					self.__selectConstraint is self.selectCameraConstraint):
					self.__selectConstraint = self.selectCameraConstraint
					if selection is not self.selectedCamera:
						self.selectedCamera = selection
						self.window.canvas.Refresh()
			elif selection is None:
				if (not self.__selectConstraint or
					self.__selectConstraint is self.deselectCameraConstraint):
					self.__selectConstraint = self.deselectCameraConstraint
					if self.selectedCamera:
						self.selectedCamera = None
						self.window.canvas.Refresh()

	# camera modifiers
	def RotateCamera(self, cursorTranslation):
		cursorTranslation *= prefs['camera.rotation.speed'] * .001
		rotation = Euler([RevToRad(c) for c in -cursorTranslation])
		self.camera.orientation *= rotation
		self.window.canvas.Refresh()

	# camera set modifiers
	def NewCamera(self):
		if not self.scene.cameraSet:
			MakeCameraSet(self.scene)
		camera = CameraSet.Camera()
		camera.position = self.camera.position
		camera.orientation = Quaternion(self.camera.orientation)
		camera.fov = self.camera.fov
		self.scene.cameraSet.cameras.append(camera)
		self.selectedCamera = camera
		self.scene.cameraSet.modified = True
		self.window.canvas.Refresh()
	def DeleteSelectedCamera(self):
		assert self.selectedCamera
		self.ClearFaceSelection()
		self.scene.cameraSet.cameras.remove(self.selectedCamera)
		self.scene.cameraSet.modified = True
		self.selectedCamera = None
		self.window.canvas.Refresh()
	def RepositionSelectedCamera(self):
		assert self.selectedCamera
		self.selectedCamera.position = self.camera.position
		self.selectedCamera.orientation = Quaternion(self.camera.orientation)
		self.scene.cameraSet.modified = True
		self.window.canvas.Refresh()
	def ShowSelectedCameraProperties(self):
		if CameraPropertiesDialog(self.window.canvas, self.selectedCamera).ShowModal():
			self.scene.cameraSet.modified = True
			self.window.canvas.Refresh()

	# track face selection modifiers
	def ClearFaceSelection(self):
		assert self.selectedCamera
		for face in self.scene.cameraSet.trackFaces:
			if self.selectedCamera in face.cameras:
				face.cameras.remove(self.selectedCamera)
		self.scene.cameraSet.modified = True
		self.window.canvas.Refresh()
	def ToggleFaceSelection(self):
		assert self.selectedCamera
		for face in self.scene.cameraSet.trackFaces:
			if self.selectedCamera in face.cameras:
				self.ClearFaceSelection()
				return
		for face in self.scene.cameraSet.trackFaces:
			face.cameras.append(self.selectedCamera)
		self.scene.cameraSet.modified = True
		self.window.canvas.Refresh()

	# file menu functions
	def NewScene(self):
		if not self.AskSave(): return
		self.scene.__init__()
		self.scene.modified = False
		self.selectedCamera = None
		self.window.canvas.Refresh()
	def OpenScene(self):
		if not self.AskSave(): return
		path = ShowOpenSceneDialog(self.window,
			os.path.dirname(prefs['file.scene.last']) + os.path.sep)
		if path:
			self.scene.FromFile(path)
			self.scene.modified = False
			if self.scene.cameraSet:
				self.scene.cameraSet.modified = False
				if self.scene.track:
					BindCameraSet(self.scene.cameraSet, self.scene.track)
			self.selectedCamera = None
			self.window.canvas.Refresh()
			prefs['file.scene.last'] = path
			print 'scene loaded from %s' % self.scene.path
	def Save(self):
		# request scene path
		if self.scene.modified and not self.scene.absPath:
			scenePath = ShowSaveSceneDialog(self.window)
			if not scenePath: return
		# request camera set path
		if self.scene.cameraSet and self.scene.cameraSet.modified and not self.scene.cameraSet.absPath:
			cameraSetPath = ShowSaveCameraSetDialog(self.window,
				(self.scene.absPath or scenePath).replace('.scene', '.cam'))
			if not cameraSetPath: return
			# NOTE: must set path here so that scene will know about it
			self.scene.cameraSet.SetPath(cameraSetPath)
		# save scene
		if self.scene.modified:
			self.scene.ToFile(self.scene.absPath or scenePath)
			self.scene.modified = False
			print 'scene saved as %s' % self.scene.path
		# save camera set
		if self.scene.cameraSet and self.scene.cameraSet.modified:
			self.scene.cameraSet.ToFile()
			self.scene.cameraSet.modified = False
			print 'camera set saved as %s' % self.scene.cameraSet.path
	def SaveAs(self):
		# request scene path
		scenePath = ShowSaveSceneDialog(self.window, self.scene.absPath)
		# request camera set path
		if self.scene.cameraSet:
			cameraSetPath = ShowSaveCameraSetDialog(self.window, self.cameraSet.absPath)
			if cameraSetPath:
				# NOTE: must set path here so that scene will know about it
				self.scene.cameraSet.SetPath(cameraSetPath)
		# save scene
		if scenePath:
			self.scene.ToFile(scenePath)
			self.scene.modified = False
			print 'scene saved as %s' % self.scene.path
		# save camera set
		if self.scene.cameraSet and cameraSetPath:
			self.scene.cameraSet.ToFile()
			self.scene.cameraSet.modified = False
			print 'camera set saved as %s' % self.scene.cameraSet.path
	def Revert(self):
		if self.scene.absPath:
			self.scene.FromFile()
			self.scene.modified = False
			if self.scene.cameraSet:
				self.scene.cameraSet.modified = False
			self.selectedCamera = None
			self.window.canvas.Refresh()
			print 'scene reverted from %s' % self.scene.path

	# event handlers
	def OnRender(self):
		self.timer.Update()
	def OnTimer(self):
		self.Update(self.timer.delta)

################################################################################
# window

class Window(BasicWindow):
	def __init__(self, app):
		BasicWindow.__init__(self, None, 'PAGE Camera Editor', (1024, 384))
		# initialize menu
		fileMenu = wx.Menu()
		fileMenu.Append(wx.ID_NEW, '&New', 'Create a new scene')
		fileMenu.Append(wx.ID_OPEN, '&Open...', 'Open an existing scene')
		fileMenu.Append(wx.ID_SAVE, '&Save', 'Save the current scene and camera set')
		fileMenu.Append(wx.ID_SAVEAS, 'Save &As...', 'Save the current scene and camera set to the specified files')
		fileMenu.Append(wx.ID_REVERT, '&Revert', 'Reload the last saved copy of the current scene and camera set')
		fileMenu.AppendSeparator()
		fileMenu.Append(wx.ID_EXIT, 'E&xit', 'Quit the program')
		menuBar = wx.MenuBar()
		menuBar.Append(fileMenu, '&File')
		self.SetMenuBar(menuBar)
		# initialize controls
		self.CreateStatusBar()
		self.canvas = Canvas(self)
		self.Show()
		# initialize event handlers
		wx.EVT_ACTIVATE(self, self.OnActivate)
		wx.EVT_CLOSE(   self, self.OnClose)
		# initialize menu event handlers
		wx.EVT_MENU(self, wx.ID_NEW,    self.OnFileNew)
		wx.EVT_MENU(self, wx.ID_OPEN,   self.OnFileOpen)
		wx.EVT_MENU(self, wx.ID_SAVE,   self.OnFileSave)
		wx.EVT_MENU(self, wx.ID_SAVEAS, self.OnFileSaveAs)
		wx.EVT_MENU(self, wx.ID_REVERT, self.OnFileRevert)
		wx.EVT_MENU(self, wx.ID_EXIT,   self.OnFileExit)
		# initialize state
		self.app = app
		self.active = True

	# event handlers
	def OnActivate(self, event):
		self.active = event.Active
		event.Skip()
	def OnClose(self, event):
		if self.app.Destroy(event.CanVeto()): self.Destroy()
		else: event.Veto()

	# menu event handlers
	def OnFileNew(   self, event): self.app.NewScene()
	def OnFileOpen(  self, event): self.app.OpenScene()
	def OnFileSave(  self, event): self.app.Save()
	def OnFileSaveAs(self, event): self.app.SaveAs()
	def OnFileRevert(self, event): self.app.Revert()
	def OnFileExit(  self, event): self.app.Exit()

################################################################################
# canvas

class Canvas(BasicCanvas):
	ID_NEW_CAMERA        = GenId()
	ID_CAMERA_PROPERTIES = GenId()
	ID_MOVE_CAMERA       = GenId()
	ID_CLEAR_CAMERA      = GenId()
	ID_DELETE_CAMERA     = GenId()

	def __init__(self, parent):
		BasicCanvas.__init__(self, parent)
		# initialize event handlers
		wx.EVT_ENTER_WINDOW(self, self.OnEnterWindow)
		wx.EVT_KEY_DOWN(   self, self.OnKeyDown)
		wx.EVT_LEFT_DOWN(  self, self.OnLeftDown)
		wx.EVT_LEFT_UP(    self, self.OnLeftUp)
		wx.EVT_MIDDLE_DOWN(self, self.OnMiddleDown)
		wx.EVT_MOTION(     self, self.OnMotion)
		wx.EVT_RIGHT_DOWN( self, self.OnRightDown)
		wx.EVT_RIGHT_UP(   self, self.OnRightUp)
		# initialize menu event handlers
		wx.EVT_MENU(self, self.ID_NEW_CAMERA,        self.OnNewCamera)
		wx.EVT_MENU(self, self.ID_CAMERA_PROPERTIES, self.OnCameraProperties)
		wx.EVT_MENU(self, self.ID_MOVE_CAMERA,       self.OnMoveCamera)
		wx.EVT_MENU(self, self.ID_CLEAR_CAMERA,      self.OnClearCamera)
		wx.EVT_MENU(self, self.ID_DELETE_CAMERA,     self.OnDeleteCamera)

	# deferred initialization
	def InitOpengl(self):
		glClearColor(.5, .5, .5, 0)
		glAlphaFunc(GL_GEQUAL, .5)
		glEnable(GL_ALPHA_TEST)
		glEnable(GL_COLOR_MATERIAL)
		glEnable(GL_CULL_FACE)
		glDepthFunc(GL_LEQUAL)
		glEnable(GL_DEPTH_TEST)
		glEnable(GL_LINE_SMOOTH)
		glEnable(GL_NORMALIZE)

	# rendering
	def Render(self):
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
		self.parent.app.RenderEdit()
		self.parent.app.RenderView()

	# context menu
	def ShowContextMenu(self):
		menu = wx.Menu()
		menu.Append(self.ID_NEW_CAMERA, '&New Camera', 'Create a new camera')
		if self.parent.app.selectedCamera:
			menu.AppendSeparator()
			menu.Append(self.ID_CAMERA_PROPERTIES, '&Properties...', 'Modify the properties of the selected camera')
			menu.Append(self.ID_MOVE_CAMERA, '&Move', 'Move the selected camera to match the current view')
			menu.Append(self.ID_CLEAR_CAMERA, '&Clear', 'Clear all track bindings for the selected camera')
			menu.Append(self.ID_DELETE_CAMERA, '&Delete', 'Delete the selected camera')
		self.PopupMenu(menu)

	# event handlers
	def OnEnterWindow(self, event):
		self.lastCursorPosition = event.Position
	def OnKeyDown(self, event):
		key = event.GetKeyCode()
		if key == wx.WXK_DELETE:
			if self.parent.app.selectedCamera:
				self.parent.app.DeleteSelectedCamera()
		elif key == wx.WXK_ESCAPE:
			self.parent.app.Exit()
		elif key == wx.WXK_SPACE:
			if self.parent.app.selectedCamera:
				self.parent.app.ToggleFaceSelection()
		elif key == wx.WXK_TAB:
			self.ShowContextMenu()
	def OnLeftDown(self, event):
		if not self.HasCapture():
			self.CaptureMouse()
		self.parent.app.UpdateSelection(event.Position, True)
	def OnLeftUp(self, event):
		if not event.RightIsDown() and self.HasCapture():
			self.ReleaseMouse()
	def OnMiddleDown(self, event):
		self.ShowContextMenu()
	def OnMotion(self, event):
		assert hasattr(self, 'lastCursorPosition')
		if event.LeftIsDown():
			self.parent.app.UpdateSelection(event.Position)
		if event.RightIsDown():
			self.parent.app.RotateCamera(
				Vector2i(event.Position) - self.lastCursorPosition)
		self.lastCursorPosition = event.Position
	def OnRightDown(self, event):
		if not self.HasCapture():
			self.CaptureMouse()
	def OnRightUp(self, event):
		if not event.LeftIsDown() and self.HasCapture():
			self.ReleaseMouse()

	# context menu event handlers
	def OnNewCamera(       self, event): self.parent.app.NewCamera()
	def OnCameraProperties(self, event): self.parent.app.ShowSelectedCameraProperties()
	def OnMoveCamera(      self, event): self.parent.app.RepositionSelectedCamera()
	def OnClearCamera(     self, event): self.parent.app.ClearFaceSelection()
	def OnDeleteCamera(    self, event): self.parent.app.DeleteSelectedCamera()

################################################################################
# camera properties dialog box

class CameraPropertiesDialog(BasicDialog):
	ID_FOV                                = GenId()
	ID_POSITION_TRACKING                  = GenId()
	ID_POSITION_TRACKING_DISTANCE         = GenId()
	ID_POSITION_TRACKING_DISTANCE_ENABLED = GenId()
	ID_ORIENTATION_TRACKING               = GenId()
	ID_TRACKING_METHOD                    = GenId()

	def __init__(self, parent, camera):
		BasicDialog.__init__(self, parent, 'Camera Properties')
		# create FOV controls
		fovSizer = wx.BoxSizer()
		fovSizer.Add(wx.StaticText(self, label = 'FOV'),
			flag = wx.RIGHT | wx.ALIGN_CENTER_VERTICAL, border = self.margin)
		fovSizer.Add(wx.TextCtrl(self, self.ID_FOV), 1)
		# create buttons
		buttonSizer = wx.StdDialogButtonSizer()
		buttonSizer.AddButton(wx.Button(self, wx.ID_OK, label = 'OK'))
		buttonSizer.AddButton(wx.Button(self, wx.ID_CANCEL, label = 'Cancel'))
		buttonSizer.Realize()
		# initialize root layout
		rootSizer = wx.BoxSizer(wx.VERTICAL)
		rootSizer.Add(fovSizer, flag = wx.ALL | wx.EXPAND, border = self.margin)
		rootSizer.Add(wx.RadioBox(self, self.ID_TRACKING_METHOD,
			'Tracking method', choices = ('None', 'Position', 'Orientation'),
			style = wx.RA_VERTICAL),
			flag = wx.BOTTOM | wx.LEFT | wx.RIGHT | wx.EXPAND,
			border = self.margin)
		rootSizer.Add(self._PositionTracking(),
			flag = wx.BOTTOM | wx.LEFT | wx.RIGHT | wx.EXPAND,
			border = self.margin)
		rootSizer.Add(self._OrientationTracking(),
			flag = wx.BOTTOM | wx.LEFT | wx.RIGHT | wx.EXPAND,
			border = self.margin)
		rootSizer.Add(buttonSizer,
			flag = wx.BOTTOM | wx.LEFT | wx.RIGHT, border = self.margin)
		self.SetSizerAndFit(rootSizer)
		# build control ID mapping
		self.controls = dict([(id, self.FindWindowById(id)) for id in (
			self.ID_FOV,
			self.ID_POSITION_TRACKING,
			self.ID_POSITION_TRACKING_DISTANCE,
			self.ID_POSITION_TRACKING_DISTANCE_ENABLED,
			self.ID_ORIENTATION_TRACKING,
			self.ID_TRACKING_METHOD)])
		# initialize command event handlers
		wx.EVT_CHECKBOX(self, self.ID_POSITION_TRACKING_DISTANCE_ENABLED, self.OnPositionTrackingDistanceEnabled)
		wx.EVT_RADIOBOX(self, self.ID_TRACKING_METHOD, self.OnTrackingMethod)
		# initialize control states
		self.controls[self.ID_FOV].Value = str(camera.fov)
		self.controls[self.ID_POSITION_TRACKING_DISTANCE].Enabled = camera.trackingDistance
		self.controls[self.ID_POSITION_TRACKING_DISTANCE].Value = str(camera.trackingDistance)
		self.controls[self.ID_POSITION_TRACKING_DISTANCE_ENABLED].Value = camera.trackingDistance
		self.controls[self.ID_TRACKING_METHOD].Selection = camera.tracking
		self.OnTrackingMethod(self.controls[self.ID_TRACKING_METHOD])
		# initialize state
		self.camera = camera

	# initialization helpers
	def _PositionTracking(self):
		root = wx.Panel(self, self.ID_POSITION_TRACKING)
		# create distance controls
		distanceSizer = wx.BoxSizer()
		distanceSizer.Add(wx.CheckBox(root,
			self.ID_POSITION_TRACKING_DISTANCE_ENABLED, 'Distance'),
			flag = wx.RIGHT | wx.ALIGN_CENTER_VERTICAL, border = self.margin)
		distanceSizer.Add(wx.TextCtrl(root, self.ID_POSITION_TRACKING_DISTANCE), 1)
		# initialize group layout
		groupSizer = wx.StaticBoxSizer(wx.StaticBox(root,
			label = 'Position tracking'), wx.VERTICAL)
		groupSizer.Add(distanceSizer)
		root.SetSizerAndFit(groupSizer)
		return root
	def _OrientationTracking(self):
		root = wx.Panel(self, self.ID_ORIENTATION_TRACKING)
		# initialize group layout
		groupSizer = wx.StaticBoxSizer(wx.StaticBox(root,
			label = 'Orientation tracking'), wx.VERTICAL)
		root.SetSizerAndFit(groupSizer)
		return root

	# show
	def ShowModal(self):
		result = wx.Dialog.ShowModal(self) == wx.ID_OK
		if result:
			try: self.camera.fov = float(self.controls[self.ID_FOV].Value or 0)
			except ValueError:
				print 'warning: non-numeric FOV; defaulting to 0'
				self.camera.fov = 0
			self.camera.tracking = self.controls[self.ID_TRACKING_METHOD].Selection
			try: self.camera.trackingDistance = (
				float(self.controls[self.ID_POSITION_TRACKING_DISTANCE].Value or 0)
				if self.controls[self.ID_POSITION_TRACKING_DISTANCE_ENABLED].Value else 0)
			except ValueError:
				print 'warning: non-numeric tracking distance; defaulting to 0'
				self.camera.trackingDistance = 0
		return result

	# event handlers
	def OnPositionTrackingDistanceEnabled(self, event):
		self.controls[self.ID_POSITION_TRACKING_DISTANCE].Enabled = event.Checked()
	def OnTrackingMethod(self, event):
		self.controls[self.ID_POSITION_TRACKING].Show(event.Selection == 1)
		self.controls[self.ID_ORIENTATION_TRACKING].Show(event.Selection == 2)
		self.Fit()

################################################################################
# camera set

def MakeCameraSet(scene):
	scene.cameraSet = CameraSet()
	scene.modified = True
	scene.cameraSet.modified = True
	if scene.track:
		scene.cameraSet.trackFaces = [CameraSet.TrackFace() for face in scene.track.faces]

def BindCameraSet(cameraSet, track):
	# rebuild binding for new track
	if hasattr(cameraSet, 'geometry'):
		# FIXME: rebind using geometry information
		# FIXME: only set modified flag if binding changed
		cameraSet.modified = True
	else:
		if len(cameraSet.trackFaces) < len(track.faces):
			cameraSet.trackFaces += [CameraSet.TrackFace()
				for i in range(len(cameraSet.trackFaces), len(track.faces))]
			cameraSet.modified = True
	# rebuild track geometry information
	if cameraSet.modified:
		pass # FIXME: implement

################################################################################
# main

def main():
	# initialize psyco
	try:
		import psyco
		psyco.full()
	except ImportError: pass
	# main loop
	Application().MainLoop()
	prefs.Commit()

if __name__ == '__main__':
	sys.exit(main() or 0)
