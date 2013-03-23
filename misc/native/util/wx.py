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

from __future__ import absolute_import

import os.path # normpath, split
from OpenGL.GL import glViewport
import wx, wx.glcanvas

################################################################################
# application

class BasicApplication(wx.PySimpleApp):
	def __init__(self):
		wx.PySimpleApp.__init__(self)

################################################################################
# window

class BasicWindow(wx.Frame):
	def __init__(self, parent = None, title = '', size = (1024, 768)):
		wx.Frame.__init__(self, parent, title = title, size = size)

################################################################################
# canvas

class BasicCanvas(wx.glcanvas.GLCanvas):
	def __init__(self, parent = None):
		wx.glcanvas.GLCanvas.__init__(self, parent, attribList = [
			wx.glcanvas.WX_GL_RGBA,
			wx.glcanvas.WX_GL_DOUBLEBUFFER,
			wx.glcanvas.WX_GL_DEPTH_SIZE, 24])
		self.parent = parent
		# initialize event handlers
		wx.EVT_ERASE_BACKGROUND(self, self.__OnEraseBackground)
		wx.EVT_PAINT(self, self.__OnPaint)
		wx.EVT_SIZE(self, self.__OnSize)
		# initialize internal state
		self._glInit = False

	# deferred initialization
	# should be overridden by derived class
	def InitOpengl(self): pass

	# rendering
	# should be overridden by derived class
	def OnRender(self): pass
	def Render(self): pass

	def SetCurrent(self):
		wx.glcanvas.GLCanvas.SetCurrent(self)
		if not self._glInit:
			self.InitOpengl()
			self._glInit = True

	# event handlers
	def __OnEraseBackground(self, event): pass
	def __OnPaint(self, event):
		self.OnRender()
		dc = wx.PaintDC(self)
		self.SetCurrent()
		self.Render()
		self.SwapBuffers()
	def __OnSize(self, event):
		self.SetCurrent()
		glViewport(0, 0, event.Size[0], event.Size[1])

################################################################################
# timer

class Timer:
	def __init__(self, callback = None, frequency = 30.):
		self.callback = callback
		self.Reset()
		# initialize internal timers
		self._timer = wx.PyTimer(self._OnTimer)
		self._stopWatch = wx.StopWatch()
		self._stopWatch.Start()
		self._timer.Start(1000. / frequency)

	# forced update
	def Update(self):
		if self._timer.IsRunning():
			self._OnTimer()

	def Reset(self):
		self.delta = 0.
		self.elapsed = 0.

	def Stop(self):
		self._timer.Stop()
		self._stopWatch.Pause()

	# event handlers
	def _OnTimer(self):
		self.delta = self._stopWatch.Time() / 1000.
		if self.delta:
			self.elapsed += self.delta
			self._stopWatch.Start()
			self._timer.Start()
			if self.callback: self.callback()

################################################################################
# dialog box

class BasicDialog(wx.Dialog):
	margin = 5

	def __init__(self, parent, title = '', style = wx.CLIP_CHILDREN | wx.DEFAULT_DIALOG_STYLE):
		wx.Dialog.__init__(self, parent, title = title, style = style)

################################################################################
# standard dialog boxes

# opening
def ShowOpenCameraSetDialog(parent, default = None):
	dirname, basename = os.path.split(default or '')
	dlg = wx.FileDialog(parent, 'Choose a camera set file to open', dirname, basename,
		'Camera set files (*.cam)|*.cam|All files (*.*)|*.*', wx.FD_OPEN)
	return os.path.normpath(dlg.Path) if dlg.ShowModal() == wx.ID_OK else None
def ShowOpenModelDialog(parent, default = None):
	dirname, basename = os.path.split(default or '')
	dlg = wx.FileDialog(parent, 'Choose a model file to open', dirname, basename,
		'Model files (*.model)|*.model|All files (*.*)|*.*', wx.FD_OPEN)
	return os.path.normpath(dlg.Path) if dlg.ShowModal() == wx.ID_OK else None
def ShowOpenSceneDialog(parent, default = None):
	dirname, basename = os.path.split(default or '')
	dlg = wx.FileDialog(parent, 'Choose a scene file to open', dirname, basename,
		'Scene files (*.scene)|*.scene|All files (*.*)|*.*', wx.FD_OPEN)
	return os.path.normpath(dlg.Path) if dlg.ShowModal() == wx.ID_OK else None

# saving
def ShowSaveCameraSetDialog(parent, default = None):
	dirname, basename = os.path.split(default or '')
	dlg = wx.FileDialog(parent, 'Choose a camera set file to save as', dirname, basename,
		'Camera set files (*.cam)|*.cam|All files (*.*)|*.*', wx.FD_SAVE | wx.FD_OVERWRITE_PROMPT)
	return os.path.normpath(dlg.Path) if dlg.ShowModal() == wx.ID_OK else None
def ShowSaveModelDialog(parent, default = None):
	dirname, basename = os.path.split(default or '')
	dlg = wx.FileDialog(parent, 'Choose a model file to save as', dirname, basename,
		'Model files (*.model)|*.model|All files (*.*)|*.*', wx.FD_SAVE | wx.FD_OVERWRITE_PROMPT)
	return os.path.normpath(dlg.Path) if dlg.ShowModal() == wx.ID_OK else None
def ShowSaveSceneDialog(parent, default = None):
	dirname, basename = os.path.split(default or '')
	dlg = wx.FileDialog(parent, 'Choose a scene file to save as', dirname, basename,
		'Scene files (*.scene)|*.scene|All files (*.*)|*.*', wx.FD_SAVE | wx.FD_OVERWRITE_PROMPT)
	return os.path.normpath(dlg.Path) if dlg.ShowModal() == wx.ID_OK else None

# saving changes
def ShowSaveChangesDialog(parent):
	result = wx.MessageBox('Save changes?', 'Exiting', wx.YES_NO | wx.CANCEL, parent)
	return {wx.YES:True, wx.NO:False, wx.CANCEL:None}[result]

################################################################################
# event ID factory

__id = wx.ID_HIGHEST

def GenId():
	global __id
	# can you say post-increment?
	id = __id
	__id += 1
	return id
