"""
Copyright (c) 2006-2012 David Osborn

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

import array
import itertools # chain
import math # tan
from OpenGL.GL import *
from OpenGL.GLU import gluBuild2DMipmaps, gluPickMatrix
import wx # Image
from .math import DegToRad, Vector3f

################################################################################
# euler operations

def MultEulerMatrix(e):
	if len(e) >= 3: glRotate(e[2], 0, 0, 1)
	glRotate(e[0], 0, 1, 0)
	if len(e) >= 2: glRotate(e[1], 1, 0, 0)

def MultInvEulerMatrix(e):
	if len(e) >= 2: glRotate(e[1], 1, 0, 0)
	glRotate(e[0], 0, 1, 0)
	if len(e) >= 3: glRotate(e[2], 0, 0, 1)

################################################################################
# quaternion operations

def MultQuatMatrix(q):
	x2, y2, z2 = Mul(q[:3], 2)
	wx, wy, wz = q[3] * x2, q[3] * y2, q[3] * z2
	xx, xy, xz = q[0] * x2, q[0] * y2, q[0] * z2
	yy, yz, zz = q[1] * y2, q[1] * z2, q[2] * z2
	glMultMatrixf((
		1 - yy - zz, xy + wz, xz - wy, 0,
		xy - wz, 1 - xx - zz, yz + wx, 0,
		xz + wy, yz - wx, 1 - xx - yy, 0,
		0, 0, 0, 1))

################################################################################
# texture

def MakeTexture(path):
	image = wx.Image(path)
	if image.HasAlpha():
		colorData = array.array('B', image.GetData()).tolist()
		alphaData = array.array('B', image.GetAlphaData()).tolist()
		data = array.array('B', itertools.chain(*zip(
			colorData[::3], colorData[1::3], colorData[2::3], alphaData))).tostring()
		format = GL_RGBA
	else:
		data = image.GetData()
		format = GL_RGB
	handle = glGenTextures(1)
	glBindTexture(GL_TEXTURE_2D, handle)
	gluBuild2DMipmaps(GL_TEXTURE_2D, format, image.GetWidth(), image.GetHeight(), format, GL_UNSIGNED_BYTE, data)
	glTexParameter(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR)
	return handle

################################################################################
# vertex array

class VertexArray:
	def __init__(self, source):
		if isinstance(source, Track):
			track = source
			self.indices = range(len(track.faces) * 3)
			# FIXME: calculate normals
			self.vertices = [value for face in track.faces
				for vertex in face.vertices for value in itertools.chain(
				(0, 0), (0, 0, 0), vertex)]
		else:
			assert isinstance(source, Mesh)
			mesh = source
			self.indices = [index for face in mesh.faces for index in face.indices]
			self.vertices = [value for vertex in mesh.vertices
				for value in itertools.chain(vertex.uv, vertex.no, vertex.co)]
		self.rawIndices = array.array('H', self.indices).tostring()
		self.rawVertices = array.array('f', self.vertices).tostring()
		# generate display list
		self.displayList = DisplayList().FromDrawable(self)

	def Update(self, mesh):
		pass # FIXME: hook for animation

	def Draw(self):
		self.displayList.Draw()

	def DrawForDisplayList(self):
		glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT)
		glTexCoordPointer(2, GL_FLOAT, 32, self.rawVertices)
		glEnableClientState(GL_TEXTURE_COORD_ARRAY)
		glNormalPointer(GL_FLOAT, 32, self.rawVertices[8:])
		glEnableClientState(GL_NORMAL_ARRAY)
		glVertexPointer(3, GL_FLOAT, 32, self.rawVertices[20:])
		glEnableClientState(GL_VERTEX_ARRAY)
		glDrawElements(GL_TRIANGLES, len(self.indices), GL_UNSIGNED_SHORT, self.rawIndices)
		glPopClientAttrib()

################################################################################
# display list

class DisplayList:
	def __init__(self):
		self.handle = glGenLists(1)

	def __del__(self):
		glDeleteLists(1, self.handle)

	def FromDrawable(self, drawable):
		glNewList(self.handle, GL_COMPILE)
		drawable.DrawForDisplayList()
		glEndList()
		return self

	def FromTrackWithNames(self, track):
		glNewList(self.handle, GL_COMPILE)
		for i, face in enumerate(track.faces):
			glPushName(i)
			glBegin(GL_TRIANGLES)
			for vertex in face.vertices:
				glVertex3fv(vertex.tolist())
			glEnd()
			glPopName()
		glEndList()
		return self

	def Draw(self):
		glCallList(self.handle)

################################################################################
# rendering support

def SetCameraMatrix(camera):
	glMatrixMode(GL_PROJECTION)
	glLoadIdentity()
	viewport = glGetInteger(GL_VIEWPORT)
	aspect = float(viewport[2]) / viewport[3]
	glMultMatrixf(camera.GetProjMatrix(aspect).tolist())
	glMatrixMode(GL_MODELVIEW)
	glLoadIdentity()
	glMultMatrixf(camera.viewMatrix.tolist())

def SetCameraMatrixForSelect(camera, pickPosition):
	glMatrixMode(GL_PROJECTION)
	glLoadIdentity()
	viewport = glGetInteger(GL_VIEWPORT)
	gluPickMatrix(pickPosition.x, viewport[3] - pickPosition.y, 1, 1, viewport)
	aspect = float(viewport[2]) / viewport[3]
	glMultMatrixf(camera.GetProjMatrix(aspect).tolist())
	glMatrixMode(GL_MODELVIEW)
	glLoadIdentity()
	glMultMatrixf(camera.viewMatrix.tolist())

def SetDefaultLighting(ambient = .25):
	glLight(GL_LIGHT0, GL_POSITION, Vector3f(1, 1, 1).Norm().tolist() + [0])
	glLight(GL_LIGHT0, GL_AMBIENT, (ambient, ambient, ambient, 1.))
	glEnable(GL_LIGHT0)
	glEnable(GL_LIGHTING)

################################################################################
# camera rendering

def DrawCamera(fov):
	d = math.tan(DegToRad(fov) / 2.)
	glBegin(GL_TRIANGLES)
	glNormal3fv(Vector3f(0, 1, d).Norm().tolist())
	glVertex3f(0., 0., 0.); glVertex3f(d, d, -1.); glVertex3f(-d, d, -1.)
	glNormal3fv(Vector3f(0, -1, d).Norm().tolist())
	glVertex3f(0., 0., 0.); glVertex3f(-d, -d, -1.); glVertex3f(d, -d, -1.)
	glNormal3fv(Vector3f(1, 0, d).Norm().tolist())
	glVertex3f(0., 0., 0.); glVertex3f(d, -d, -1.); glVertex3f(d, d, -1.)
	glNormal3fv(Vector3f(-1, 0, d).Norm().tolist())
	glVertex3f(0., 0., 0.); glVertex3f(-d, d, -1.); glVertex3f(-d, -d, -1.)
	glNormal3f(0., 0., -1.)
	glVertex3f(-d, -d, -1.); glVertex3f(d, d, -1.); glVertex3f(d, -d, -1.)
	glVertex3f(-d, -d, -1.); glVertex3f(-d, d, -1.); glVertex3f(d, d, -1.)
	glEnd()

def DrawCameraFrustum(fov, far):
	d = math.tan(DegToRad(fov) / 2.)
	glBegin(GL_LINE_LOOP)
	glVertex3f(-d, -d, -1.)
	glVertex3f(-d,  d, -1.)
	glVertex3f( d,  d, -1.)
	glVertex3f( d, -d, -1.)
	glEnd()
	glMatrixMode(GL_MODELVIEW)
	glPushMatrix()
	glScalef(far, far, far)
	glBegin(GL_LINES)
	glVertex3f(0., 0., 0.); glVertex3f(-d, -d, -1.)
	glVertex3f(0., 0., 0.); glVertex3f(-d,  d, -1.)
	glVertex3f(0., 0., 0.); glVertex3f( d,  d, -1.)
	glVertex3f(0., 0., 0.); glVertex3f( d, -d, -1.)
	glEnd()
	glBegin(GL_LINE_LOOP)
	glVertex3f(-d, -d, -1.)
	glVertex3f(-d,  d, -1.)
	glVertex3f( d,  d, -1.)
	glVertex3f( d, -d, -1.)
	glEnd()
	glPopMatrix()

def DrawCameras(cameras, selected = (), selectColor = None):
	glPushAttrib(GL_CURRENT_BIT | GL_LIGHTING_BIT)
	SetDefaultLighting(.5)
	for camera in cameras:
		glMatrixMode(GL_MODELVIEW)
		glPushMatrix()
		glMultMatrixf(camera.matrix.tolist())
		# draw camera
		if camera in selected: glColor3fv(selectColor or (1., 1., .5))
		else: glColor3f(.5, .5, .5)
		DrawCamera(camera.fov)
		# draw frustum
		glColor3f(0., 0., 0.)
		DrawCameraFrustum(camera.fov, 5)
		glMatrixMode(GL_MODELVIEW)
		glPopMatrix()
	glPopAttrib()

def DrawCamerasForSelect(cameras):
	for i, camera in enumerate(cameras):
		glMatrixMode(GL_MODELVIEW)
		glPushMatrix()
		glMultMatrixf(camera.matrix.tolist())
		glPushName(i)
		DrawCamera(camera.fov)
		glPopName()
		glMatrixMode(GL_MODELVIEW)
		glPopMatrix()

################################################################################
# scene rendering

def DrawScene(scene, selectedForms = (), selectColor = None):
	glPushAttrib(GL_LIGHTING_BIT)
	SetDefaultLighting()
	DrawForms(scene.forms, selectedForms, selectColor)
	glPopAttrib()

def DrawForms(forms, selected = (), selectColor = None):
	for form in forms:
		glPushAttrib(GL_CURRENT_BIT)
		if form in selected: glColor3fv(selectColor or (1., .5, .5))
		DrawForm(form)
		glPopAttrib()

def DrawForm(form, selectedParts = (), selectColor = None):
	if form.model:
		glMatrixMode(GL_MODELVIEW)
		glPushMatrix()
		glMultMatrixf(form.transformedMatrix.tolist())
		DrawParts(form.model.parts, selectedParts, selectColor)
		glMatrixMode(GL_MODELVIEW)
		glPopMatrix()

def DrawParts(parts, selected = (), selectColor = None):
	for part in parts:
		glPushAttrib(GL_CURRENT_BIT)
		if part in selected: glColor3fv(selectColor or (1., .5, .5))
		DrawPart(part)
		glPopAttrib()

def DrawPart(part):
	if part.mesh:
		glPushAttrib(GL_ENABLE_BIT)
		if part.material and part.material.passes and \
			part.material.passes[0].diffuse.texture.texture:
			glBindTexture(GL_TEXTURE_2D, part.material.passes[0].diffuse.texture.texture.handle)
			glEnable(GL_TEXTURE_2D)
		part.mesh.GetDrawable().Draw()
		glPopAttrib()

def DrawSceneWireframe(scene):
	glPushAttrib(GL_CURRENT_BIT | GL_POLYGON_BIT)
	glPolygonMode(GL_FRONT, GL_LINE)
	glColor(0, 0, 0)
	for form in scene.forms:
		if form.model:
			glMatrixMode(GL_MODELVIEW)
			glPushMatrix()
			glMultMatrixf(form.matrix.tolist())
			for part in form.model.parts:
				part.mesh.GetDrawable().Draw()
			glMatrixMode(GL_MODELVIEW)
			glPopMatrix()
	glPopAttrib()

def DrawSceneForSelect(scene):
	for i, form in enumerate(scene.forms):
		if form.model:
			glMatrixMode(GL_MODELVIEW)
			glPushMatrix()
			glMultMatrixf(form.matrix.tolist())
			glPushName(i)
			for part in form.model.parts:
				part.mesh.GetDrawable().Draw()
			glPopName()
			glMatrixMode(GL_MODELVIEW)
			glPopMatrix()

################################################################################
# track rendering

def DrawTrack(track, selectedFaces = (), overlay = True):
	glPushAttrib(GL_CURRENT_BIT | GL_DEPTH_BUFFER_BIT | GL_ENABLE_BIT | GL_POLYGON_BIT)
	glDisable(GL_CULL_FACE)
	if overlay:
		# offset depth to avoid Z fighting
		'''
		matrix = glGetFloatv(GL_MODELVIEW_MATRIX)
		glMatrixMode(GL_MODELVIEW)
		glPushMatrix()
		glLoadIdentity()
		glTranslatef(0., 0., .01)
		glMultMatrixf(matrix)
		'''
		glMatrixMode(GL_MODELVIEW)
		glPushMatrix()
		glTranslatef(0., .01, 0.)
		# initialize compositing state
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)
		glEnable(GL_BLEND)
		glColor4f(.75, .75, .75, .5)
	else:
		glColor3f(.5, .5, .5)
	track.GetDrawable().Draw()
	# draw selected faces
	glDepthMask(GL_FALSE)
	if selectedFaces:
		if overlay: glColor4f(1, .5, .5, .5)
		else: glColor3f(1, .5, .5)
		glBegin(GL_TRIANGLES)
		for face in selectedFaces:
			for vertex in face.vertices:
				glVertex3fv(vertex.tolist())
		glEnd()
	# draw wireframe
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE)
	if overlay: glDisable(GL_BLEND)
	glColor3f(0., 0., 0.)
	track.GetDrawable().Draw()
	if overlay:
		glMatrixMode(GL_MODELVIEW)
		glPopMatrix()
	glPopAttrib()

def DrawTrackForSelect(track):
	glPushAttrib(GL_ENABLE_BIT)
	glDisable(GL_CULL_FACE)
	track.GetSelectDrawable().Draw()
	glPopAttrib()

################################################################################
# circular imports

from .res import Mesh, Track
