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

import os.path # {base,dir}name, sep, splitext
import sys # exit
from OpenGL.GL import *
import wx, wx.glcanvas
from util.config import prefs
from util.math import Barycentric, Bilerp, Euler, EulerBetween, DegToRad, IsPointInTriangle, NegInf, RevToRad, TranslationMatrix, Vector2i, Vector3f
from util.opengl import DrawScene, DrawSceneForSelect, DrawSceneWireframe, SetCameraMatrix, SetCameraMatrixForSelect
from util.phys import Camera, Orientation
from util.res import Model, Scene
from util.wx import BasicApplication, BasicCanvas, BasicDialog, BasicWindow, GenId, ShowOpenModelDialog, ShowOpenSceneDialog, ShowSaveChangesDialog, ShowSaveSceneDialog, Timer

################################################################################
# application

class Application(BasicApplication):
	# modes
	SELECT_MODE         = 0
	MOVE_MODE           = 1
	MOVE_X_MODE         = 2
	MOVE_Y_MODE         = 3
	MOVE_Z_MODE         = 4
	ROTATE_X_MODE       = 6
	ROTATE_Y_MODE       = 7
	ROTATE_Z_MODE       = 8
	SCALE_MODE          = 9
	SCALE_X_MODE        = 10
	SCALE_Y_MODE        = 11
	SCALE_Z_MODE        = 12
	SCALE_RADIAL_X_MODE = 13
	SCALE_RADIAL_Y_MODE = 14
	SCALE_RADIAL_Z_MODE = 15

	def __init__(self):
		BasicApplication.__init__(self)
		# initialize timer
		self.timer = Timer(self.OnTimer)
		# initialize state
		self.scene = Scene()
		self.scene.modified = False
		self.camera = ZoomCamera()
		self.selected = []
		self.mode = self.SELECT_MODE
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
				if wx.GetKeyState(wx.WXK_CONTROL): translation *= .25
				if wx.GetKeyState(wx.WXK_SHIFT): translation *= 4
				translation.x, translation.z = (
					Euler(self.camera.orientation.yaw, 0, 0) *
					Vector3f(translation.x, 0, translation.z)).Swizzle(0, 2)
				self.camera.position += (translation *
					prefs['camera.translation.speed'] * deltaTime)
				self.window.canvas.Refresh()

	def AskSave(self):
		if self.scene.modified:
			result = ShowSaveChangesDialog(self.window)
			if result: self.SaveScene()
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
	def Render(self):
		SetCameraMatrix(self.camera)
		DrawScene(self.scene, self.selected,
			(1., .5, .5) if self.mode == self.SELECT_MODE else (1., .75, .5))
		# draw wireframe
		if prefs['render.wireframe']:
			glPushAttrib(GL_DEPTH_BUFFER_BIT)
			glDepthMask(False)
			DrawSceneWireframe(self.scene)
			glPopAttrib()
		# draw world axes
		if prefs['render.axes']:
			self.DrawAxes()
	def DrawAxes(self):
		glTranslate(*self.camera.position)
		glPushAttrib(GL_CURRENT_BIT | GL_DEPTH_BUFFER_BIT)
		glDepthMask(False)
		glDepthFunc(GL_GEQUAL)
		glBegin(GL_LINES)
		axes = (
			Vector3f(1, 0, 0),
			Vector3f(0, 1, 0),
			Vector3f(0, 0, 1))
		for axis in axes:
			glColor((axis * 0.5).tolist())
			glVertex(0, 0, 0)
			glVertex((axis * 0.5).tolist())
		glEnd()
		glDepthMask(True)
		glDepthFunc(GL_LESS)
		glBegin(GL_LINES)
		for axis in axes:
			glColor(axis.tolist())
			glVertex(0, 0, 0)
			glVertex((axis * 0.5).tolist())
		glEnd()
		glPopAttrib()

	# selection
	def RenderSelect(self, cursorPosition):
		SetCameraMatrixForSelect(self.camera, cursorPosition)
		# initialize selection mode
		glSelectBuffer(256)
		glRenderMode(GL_SELECT)
		glInitNames()
		# draw scene
		DrawSceneForSelect(self.scene)
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
				return self.scene.forms[names[0]]
		return None
	def UpdateSelection(self, cursorPosition, shiftDown, ctrlDown):
		selection = self.Select(cursorPosition)
		if selection:
			if shiftDown:
				if ctrlDown:
					if selection in self.selected:
						self.selected.remove(selection)
						self.window.canvas.Refresh()
				elif selection not in self.selected:
					self.selected.append(selection)
					self.window.canvas.Refresh()
			elif [selection] != self.selected:
				self.selected = [selection]
				self.window.canvas.Refresh()
		elif not shiftDown and self.selected:
			self.selected = []
			self.window.canvas.Refresh()

	# mode functions
	def SetMode(self, mode):
		self.mode = mode
		self.window.canvas.Refresh()
	def BakeMode(self):
		if self.mode != self.SELECT_MODE:
			for sel in self.selected:
				sel.BakeTransformation()
			self.SetMode(self.SELECT_MODE)
			self.scene.modified = True
	def ResetMode(self):
		if self.mode != self.SELECT_MODE:
			for sel in self.selected:
				sel.ResetTransformation()
			self.SetMode(self.SELECT_MODE)
	def ModeOnCursorMotion(self, motion):
		if self.mode == self.MOVE_MODE:
			translation = (
				Euler(self.camera.orientation.yaw, 0, 0) *
				Vector3f(motion.x, 0, motion.y) *
				prefs['tool.translation.speed'])
			for sel in self.selected:
				sel.translation += translation
		else:
			translation = motion.x * prefs['tool.translation.speed']
			rotation = motion.x * prefs['tool.rotation.speed']
			scale = motion.x * prefs['tool.scale.speed']
			if self.mode == self.MOVE_X_MODE:
				for sel in self.selected:
					sel.translation.x += translation
			elif self.mode == self.MOVE_Y_MODE:
				for sel in self.selected:
					sel.translation.y += translation
			elif self.mode == self.MOVE_Z_MODE:
				for sel in self.selected:
					sel.translation.z += translation
			elif self.mode == self.ROTATE_X_MODE:
				for sel in self.selected:
					sel.rotation.pitch += rotation
			elif self.mode == self.ROTATE_Y_MODE:
				for sel in self.selected:
					sel.rotation.yaw += rotation
			elif self.mode == self.ROTATE_Z_MODE:
				for sel in self.selected:
					sel.rotation.roll += rotation
			elif self.mode == self.SCALE_MODE:
				for sel in self.selected:
					sel.rescale += scale
			elif self.mode == self.SCALE_X_MODE:
				for sel in self.selected:
					sel.rescale.x += scale
			elif self.mode == self.SCALE_Y_MODE:
				for sel in self.selected:
					sel.rescale.y += scale
			elif self.mode == self.SCALE_Z_MODE:
				for sel in self.selected:
					sel.rescale.z += scale
			elif self.mode == self.SCALE_RADIAL_X_MODE:
				for sel in self.selected:
					sel.rescale.y += scale
					sel.rescale.z += scale
			elif self.mode == self.SCALE_RADIAL_Y_MODE:
				for sel in self.selected:
					sel.rescale.x += scale
					sel.rescale.z += scale
			elif self.mode == self.SCALE_RADIAL_Z_MODE:
				for sel in self.selected:
					sel.rescale.x += scale
					sel.rescale.y += scale
		self.window.canvas.Refresh()

	# camera modifiers
	def RotateCamera(self, cursorTranslation):
		cursorTranslation *= prefs['camera.rotation.speed']
		rotation = Euler([RevToRad(c) for c in -cursorTranslation])
		self.camera.orientation *= rotation
		self.window.canvas.Refresh()
	def GotoSelection(self):
		if self.selected:
			self.camera.position = \
				sum([sel.position for sel in self.selected]) / len(self.selected)
			self.window.canvas.Refresh()

	# scene modifiers
	def NewForm(self):
		path = ShowOpenModelDialog(self.window,
			os.path.dirname(prefs['file.model.last']) + os.path.sep)
		if path:
			model = Model().FromFile(path)
			name = os.path.splitext(os.path.basename(path))[0]
			form = self.scene.Form(name, model)
			form.position = self.camera.position
			form.orientation.yaw = self.camera.orientation.yaw
			self.scene.forms.append(form)
			self.scene.modified = True
			self.window.canvas.Refresh()
			prefs['file.model.last'] = path

	# absolute positioning
	def PutOnTrack(self):
		# FIXME: implement
		self.window.canvas.Refresh()
	def PutOnGround(self):
		print 'working...'
		for sel in self.selected:
			if sel.name:
				print '  %s...' % sel.name
			nearest = (NegInf, None)
			for form in self.scene.forms:
				if form.model and form not in self.selected:
					for part in form.model.parts:
						vertices = [form.matrix * vertex.co for vertex in part.mesh.vertices]
						for face in part.mesh.faces:
							if IsPointInTriangle(
								sel.position.Swizzle(0, 2),
								vertices[face.indices[0]].Swizzle(0, 2),
								vertices[face.indices[1]].Swizzle(0, 2),
								vertices[face.indices[2]].Swizzle(0, 2)):
								mu = Barycentric(
									sel.position.Swizzle(0, 2),
									vertices[face.indices[0]].Swizzle(0, 2),
									vertices[face.indices[1]].Swizzle(0, 2),
									vertices[face.indices[2]].Swizzle(0, 2))
								height = Bilerp(
									vertices[face.indices[0]].y,
									vertices[face.indices[1]].y,
									vertices[face.indices[2]].y, *mu)
								if (height <= sel.position.y + .001 and
									height > nearest[0]):
									normal = Bilerp(
										Orientation.GetMatrix(form) * part.mesh.vertices[face.indices[0]].no,
										Orientation.GetMatrix(form) * part.mesh.vertices[face.indices[1]].no,
										Orientation.GetMatrix(form) * part.mesh.vertices[face.indices[2]].no, *mu).Norm()
									nearest = (height, normal)
			if nearest[1]:
				sel.position.y = nearest[0]
				# FIXME: this implementation doesn't work
#				sel.orientation = EulerBetween(Vector3f(0, 1, 0), normal)
		self.window.canvas.Refresh()
		print 'done'

	# selection modifiers
	def DeleteSelection(self):
		if self.selected:
			for sel in self.selected:
				self.scene.forms.remove(sel)
			self.selected = []
			self.window.canvas.Refresh()
	def ToggleSelection(self):
		for form in self.scene.forms:
			if form in self.selected:
				self.selected = []
				break
		else:
			self.selected = self.scene.forms
		self.window.canvas.Refresh()

	# file menu functions
	def NewScene(self):
		if not self.AskSave(): return
		self.scene.__init__()
		self.scene.modified = False
		self.selected = []
		self.window.canvas.Refresh()
	def OpenScene(self):
		if not self.AskSave(): return
		path = ShowOpenSceneDialog(self.window,
			os.path.dirname(prefs['file.scene.last']) + os.path.sep)
		if path:
			self.scene.FromFile(path)
			self.scene.modified = False
			self.selected = []
			self.window.canvas.Refresh()
			prefs['file.scene.last'] = path
			print 'scene loaded from %s' % self.scene.path
	def SaveScene(self):
		if self.scene.modified:
			if self.scene.absPath:
				self.scene.ToFile()
				self.scene.modified = False
				print 'scene saved as %s' % self.scene.path
			else:
				self.SaveSceneAs()
	def SaveSceneAs(self):
		scenePath = ShowSaveSceneDialog(self.window, self.scene.absPath)
		if scenePath:
			self.scene.ToFile(scenePath)
			self.scene.modified = False
			print 'scene saved as %s' % self.scene.path
	def RevertScene(self):
		if self.scene.absPath:
			self.scene.FromFile()
			self.scene.modified = False
			self.selected = []
			self.window.canvas.Refresh()
			print 'scene reverted from %s' % self.scene.path
	def ChangeRootDir(self):
		# FIXME: this should be a standard dialog
		# FIXME: we will have to reload all resources if root is changed
		pass
		'''
		dlg = wx.DirDialog(self, 'Choose a root directory for loading resources', prefs['res.root'], wx.DD_DEFAULT_STYLE | wx.DD_DIR_MUST_EXIST)
		if dlg.ShowModal() == wx.ID_OK:
			prefs['res.root'] = dlg.GetPath()
		'''
	def ShowPreferences(self):
		pass # FIXME: show preference dialog

	# event handlers
	def OnRender(self):
		self.timer.Update()
	def OnTimer(self):
		self.Update(self.timer.delta)

################################################################################
# window

class Window(BasicWindow):
	ID_RECENT  = GenId()
	ID_ROOTDIR = GenId()
	ID_PREFS   = GenId()

	def __init__(self, app):
		BasicWindow.__init__(self, None, 'PAGE Scene Editor')
		# initialize menu
		fileMenu = wx.Menu()
		fileMenu.Append(wx.ID_NEW, '&New', 'Create a new scene')
		fileMenu.Append(wx.ID_OPEN, '&Open...', 'Open an existing scene')
		fileMenu.Append(wx.ID_SAVE, '&Save', 'Save the current scene')
		fileMenu.Append(wx.ID_SAVEAS, 'Save &As...', 'Save the current scene to a specified file')
		fileMenu.Append(wx.ID_REVERT, '&Revert', 'Reload the last saved copy of the current scene')
		fileMenu.AppendSeparator()
		fileMenu.Append(self.ID_ROOTDIR, 'Root &Directory...', 'Set the root directory for loading resources')
		fileMenu.Append(self.ID_PREFS, '&Preferences...', 'Change editor options')
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
		wx.EVT_MENU(self,   wx.ID_NEW,     self.OnFileNew)
		wx.EVT_MENU(self,   wx.ID_OPEN,    self.OnFileOpen)
		wx.EVT_MENU(self,   wx.ID_SAVE,    self.OnFileSave)
		wx.EVT_MENU(self,   wx.ID_SAVEAS,  self.OnFileSaveAs)
		wx.EVT_MENU(self,   wx.ID_REVERT,  self.OnFileRevert)
		wx.EVT_MENU(self, self.ID_ROOTDIR, self.OnFileRootDir)
		wx.EVT_MENU(self, self.ID_PREFS,   self.OnFilePrefs)
		wx.EVT_MENU(self,   wx.ID_EXIT,    self.OnFileExit)
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
	def OnFileNew(    self, event): self.app.NewScene()
	def OnFileOpen(   self, event): self.app.OpenScene()
	def OnFileSave(   self, event): self.app.SaveScene()
	def OnFileSaveAs( self, event): self.app.SaveSceneAs()
	def OnFileRevert( self, event): self.app.RevertScene()
	def OnFileRootDir(self, event): self.app.ChangeRootDir()
	def OnFilePrefs(  self, event): self.app.ShowPreferences()
	def OnFileExit(   self, event): self.app.Exit()

################################################################################
# canvas

class Canvas(BasicCanvas):
	ID_ADD            = GenId()
	ID_MOVE           = GenId()
	ID_MOVE_X         = GenId()
	ID_MOVE_Y         = GenId()
	ID_MOVE_Z         = GenId()
	ID_ROTATE_X       = GenId()
	ID_ROTATE_Y       = GenId()
	ID_ROTATE_Z       = GenId()
	ID_SCALE          = GenId()
	ID_SCALE_X        = GenId()
	ID_SCALE_Y        = GenId()
	ID_SCALE_Z        = GenId()
	ID_SCALE_RADIAL_X = GenId()
	ID_SCALE_RADIAL_Y = GenId()
	ID_SCALE_RADIAL_Z = GenId()
	ID_PUT_ON_TRACK   = GenId()
	ID_PUT_ON_GROUND  = GenId()
	ID_DELETE         = GenId()

	def __init__(self, parent):
		BasicCanvas.__init__(self, parent)
		# initialize event handlers
		wx.EVT_ENTER_WINDOW(self, self.OnEnterWindow)
		wx.EVT_KEY_DOWN(    self, self.OnKeyDown)
		wx.EVT_LEFT_DOWN(   self, self.OnLeftDown)
		wx.EVT_MOTION(      self, self.OnMotion)
		wx.EVT_MOUSEWHEEL(  self, self.OnMousewheel)
		wx.EVT_RIGHT_DOWN(  self, self.OnRightDown)
		# initialize menu event handlers
		wx.EVT_MENU(self, self.ID_ADD,            self.OnAdd)
		wx.EVT_MENU(self, self.ID_MOVE,           self.OnMove)
		wx.EVT_MENU(self, self.ID_MOVE_X,         self.OnMoveX)
		wx.EVT_MENU(self, self.ID_MOVE_Y,         self.OnMoveY)
		wx.EVT_MENU(self, self.ID_MOVE_Z,         self.OnMoveZ)
		wx.EVT_MENU(self, self.ID_ROTATE_X,       self.OnRotateX)
		wx.EVT_MENU(self, self.ID_ROTATE_Y,       self.OnRotateY)
		wx.EVT_MENU(self, self.ID_ROTATE_Z,       self.OnRotateZ)
		wx.EVT_MENU(self, self.ID_SCALE,          self.OnScale)
		wx.EVT_MENU(self, self.ID_SCALE_X,        self.OnScaleX)
		wx.EVT_MENU(self, self.ID_SCALE_Y,        self.OnScaleY)
		wx.EVT_MENU(self, self.ID_SCALE_Z,        self.OnScaleZ)
		wx.EVT_MENU(self, self.ID_SCALE_RADIAL_X, self.OnScaleRadialX)
		wx.EVT_MENU(self, self.ID_SCALE_RADIAL_Y, self.OnScaleRadialY)
		wx.EVT_MENU(self, self.ID_SCALE_RADIAL_Z, self.OnScaleRadialZ)
		wx.EVT_MENU(self, self.ID_PUT_ON_TRACK,   self.OnPutOnTrack)
		wx.EVT_MENU(self, self.ID_PUT_ON_GROUND,  self.OnPutOnGround)
		wx.EVT_MENU(self, self.ID_DELETE,         self.OnDelete)

	# deferred initialization
	def InitOpengl(self):
		glClearColor(.5, .5, .5, 0.)
		glAlphaFunc(GL_GEQUAL, .5)
		glEnable(GL_ALPHA_TEST)
		glEnable(GL_COLOR_MATERIAL)
		glEnable(GL_CULL_FACE)
		glEnable(GL_DEPTH_TEST)
		glEnable(GL_LINE_SMOOTH)
		glEnable(GL_NORMALIZE)

	# rendering
	def Render(self):
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
		self.parent.app.Render()

	# context menu
	def ShowContextMenu(self):
		menu = wx.Menu()
		if self.parent.app.selected:
			moveMenu = wx.Menu()
			moveMenu.Append(self.ID_MOVE, '&Horizontal', 'Move the selected objects over the XZ plane')
			moveMenu.Append(self.ID_MOVE_X, '&X', 'Move the selected objects along the X axis')
			moveMenu.Append(self.ID_MOVE_Y, '&Y', 'Move the selected objects along the Y axis')
			moveMenu.Append(self.ID_MOVE_Z, '&Z', 'Move the selected objects along the Z axis')
			rotateMenu = wx.Menu()
			rotateMenu.Append(self.ID_ROTATE_X, '&X', 'Rotate the selected objects around the X axis')
			rotateMenu.Append(self.ID_ROTATE_Y, '&Y', 'Rotate the selected objects around the Y axis')
			rotateMenu.Append(self.ID_ROTATE_Z, '&Z', 'Rotate the selected objects around the Z axis')
			scaleMenu = wx.Menu()
			scaleMenu.Append(self.ID_SCALE, '&Uniform', 'Scale the selected objects')
			scaleMenu.Append(self.ID_SCALE_X, '&X', 'Scale the selected objects along the X axis')
			scaleMenu.Append(self.ID_SCALE_Y, '&Y', 'Scale the selected objects along the Y axis')
			scaleMenu.Append(self.ID_SCALE_Z, '&Z', 'Scale the selected objects along the Z axis')
			scaleMenu.Append(self.ID_SCALE_RADIAL_X, 'Radial X', 'Scale the selected objects over the YZ plane')
			scaleMenu.Append(self.ID_SCALE_RADIAL_Y, 'Radial Y', 'Scale the selected objects over the XZ plane')
			scaleMenu.Append(self.ID_SCALE_RADIAL_Z, 'Radial Z', 'Scale the selected objects over the XY plane')
			menu.AppendSubMenu(moveMenu, '&Move', 'Move the selected objects')
			menu.AppendSubMenu(rotateMenu, '&Rotate', 'Rotate the selected objects')
			menu.AppendSubMenu(scaleMenu, '&Scale', 'Scale the selected objects')
			menu.AppendSeparator()
			menu.Append(self.ID_PUT_ON_TRACK, 'Put on &track', 'Project the selected objects onto the track')
			menu.Append(self.ID_PUT_ON_GROUND, 'Put on &ground', 'Project the selected objects onto the ground')
			menu.AppendSeparator()
			menu.Append(self.ID_DELETE, '&Delete', 'Remove the selected objects from the scene')
		else:
			menu.Append(self.ID_ADD, '&Add...', 'Add a new object to the scene')
		self.PopupMenu(menu)

	# event handlers
	def OnEnterWindow(self, event):
		self.lastCursorPosition = event.Position
	def OnKeyDown(self, event):
		key = event.GetKeyCode()
		if key == wx.WXK_TAB:
			self.ShowContextMenu()
		elif key == wx.WXK_RETURN:
			self.parent.app.GotoSelection()
		elif key == wx.WXK_SPACE:
			self.parent.app.ToggleSelection()
		elif self.parent.app.selected:
			if key == wx.WXK_ESCAPE: self.parent.app.ResetMode()
			elif key == wx.WXK_RETURN: self.parent.app.BakeMode()
			elif key == wx.WXK_DELETE or key == wx.WXK_BACK:
				self.parent.app.DeleteSelection()
			elif key == ord('M'): self.parent.app.SetMode(self.parent.app.MOVE_MODE)
			elif key == ord('X'): self.parent.app.SetMode(self.parent.app.MOVE_X_MODE)
			elif key == ord('Y'): self.parent.app.SetMode(self.parent.app.MOVE_Y_MODE)
			elif key == ord('Z'): self.parent.app.SetMode(self.parent.app.MOVE_Z_MODE)
			elif key == ord('R'): self.parent.app.SetMode(self.parent.app.ROTATE_Y_MODE)
			elif key == ord('U'): self.parent.app.SetMode(self.parent.app.SCALE_MODE)
			elif key == ord('T'): self.parent.app.PutOnTrack()
			elif key == ord('G'): self.parent.app.PutOnGround()
		event.Skip()
	def OnLeftDown(self, event):
		if self.parent.app.mode == self.parent.app.SELECT_MODE:
			self.parent.app.UpdateSelection(event.Position,
				wx.GetKeyState(wx.WXK_SHIFT), wx.GetKeyState(wx.WXK_CONTROL))
		else:
			self.parent.app.BakeMode()
	def OnMotion(self, event):
		assert hasattr(self, 'lastCursorPosition')
		motion = (Vector2i(event.Position) - self.lastCursorPosition) * .001
		if self.parent.app.mode == self.parent.app.SELECT_MODE:
			if event.LeftIsDown():
				self.parent.app.UpdateSelection(event.Position,
					wx.GetKeyState(wx.WXK_SHIFT), wx.GetKeyState(wx.WXK_CONTROL))
			if event.RightIsDown():
				self.parent.app.RotateCamera(motion)
		else:
			if wx.GetKeyState(wx.WXK_CONTROL): motion *= .25
			if wx.GetKeyState(wx.WXK_SHIFT): motion *= 4
			self.parent.app.ModeOnCursorMotion(motion)
		self.lastCursorPosition = event.Position
	def OnMousewheel(self, event):
		rotation = float(event.GetWheelRotation()) / event.GetWheelDelta()
		if rotation:
			if wx.GetKeyState(wx.WXK_SHIFT): rotation *= 2
			self.parent.app.camera.Zoom(rotation * prefs['camera.scale.speed'])
			self.Refresh()
	def OnRightDown(self, event):
		self.parent.app.ResetMode()

	# menu event handlers
	def OnAdd(         self, event): self.parent.app.NewForm()
	def OnMove(        self, event): self.parent.app.SetMode(self.parent.app.MOVE_MODE)
	def OnMoveX(       self, event): self.parent.app.SetMode(self.parent.app.MOVE_X_MODE)
	def OnMoveY(       self, event): self.parent.app.SetMode(self.parent.app.MOVE_Y_MODE)
	def OnMoveZ(       self, event): self.parent.app.SetMode(self.parent.app.MOVE_Z_MODE)
	def OnRotateX(     self, event): self.parent.app.SetMode(self.parent.app.ROTATE_X_MODE)
	def OnRotateY(     self, event): self.parent.app.SetMode(self.parent.app.ROTATE_Y_MODE)
	def OnRotateZ(     self, event): self.parent.app.SetMode(self.parent.app.ROTATE_Z_MODE)
	def OnScale(       self, event): self.parent.app.SetMode(self.parent.app.SCALE_MODE)
	def OnScaleX(      self, event): self.parent.app.SetMode(self.parent.app.SCALE_X_MODE)
	def OnScaleY(      self, event): self.parent.app.SetMode(self.parent.app.SCALE_Y_MODE)
	def OnScaleZ(      self, event): self.parent.app.SetMode(self.parent.app.SCALE_Z_MODE)
	def OnScaleRadialX(self, event): self.parent.app.SetMode(self.parent.app.SCALE_RADIAL_X_MODE)
	def OnScaleRadialY(self, event): self.parent.app.SetMode(self.parent.app.SCALE_RADIAL_Y_MODE)
	def OnScaleRadialZ(self, event): self.parent.app.SetMode(self.parent.app.SCALE_RADIAL_Z_MODE)
	def OnPutOnTrack(  self, event): self.parent.app.PutOnTrack()
	def OnPutOnGround( self, event): self.parent.app.PutOnGround()
	def OnDelete(      self, event): self.parent.app.DeleteSelection()

################################################################################
# zoom camera

class ZoomCamera(Camera):
	def __init__(self, position = None, orientation = None, zoom = 10.):
		if orientation is None: orientation = Euler(DegToRad(45), DegToRad(-45))
		Camera.__init__(self, position, orientation)
		self.zoom = zoom

	def GetViewMatrix(self):
		return (
			TranslationMatrix(Vector3f(0, 0, -self.zoom)) *
			Camera.GetViewMatrix(self))

	def Translate(self, translation):
		self.position += Vector3f(translation)
	def Rotate(self, rotation):
		self.orientation += Euler(rotation)
	def Zoom(self, scale):
		self.zoom = min(max(self.zoom - scale, 1), 20)

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
