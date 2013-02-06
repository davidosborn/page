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

import os.path # exists, join, norm{case,path}
import re # sub
import struct # {,un}pack
from OpenGL.GL import *
from .config import prefs # res.root
from .error import FormatError, PathError
from .io import LineIterator
from .math import DegToRad, Euler, ProjMatrix, Quaternion, RgbColorf, RotationMatrix, TranslationMatrix, Vector2f, Vector3f
from .opengl import DisplayList, MakeTexture, VertexArray
from .phys import PositionOrientationScale

################################################################################
# source

def FindResource(path):
	sources = prefs['res.source']
	if isinstance(sources, str):
		sources = [sources]
	for source in prefs['res.source']:
		absPath = os.path.normcase(os.path.normpath(os.path.join(os.path.normpath(source), path)))
		if os.path.exists(absPath):
			return absPath
	raise PathError('invalid path: %s' % path)

def LoadResource(type, path):
	print 'loading %s from %s' % (GetTypeName(type), path)
	res = type().FromFile(FindResource(path), path)
	return res

def GetTypeName(type):
	name = str(type).split('\'', 2)[1].rsplit('.', 1)[-1]
	return re.sub(r'([a-z])([A-Z])', r'\1 \2', name).lower()

def GetSourcePath(path):
	path = os.path.normcase(os.path.normpath(path))
	for source in prefs['res.source']:
		sourcePath = os.path.normcase(os.path.normpath(source))
		common = os.path.commonprefix((sourcePath, path))
		if common == sourcePath:
			return path.replace(common, '')[1:].replace(os.path.sep, '/')
	return None

################################################################################
# cache

# FIXME: implement

################################################################################
# native text format

class Parser:
	# constraints
	noValue    = 0
	allowValue = 1
	needValue  = 2

	class Handler:
		def __init__(self, callback, constraint):
			self.callback = callback
			self.constraint = constraint

	def __init__(self):
		self.handlers = {}

	def Register(self, key, callback = None, constraint = allowValue):
		self.handlers[key] = self.Handler(callback, constraint)

	def Parse(self, path):
		stack = []
		lines = LineIterator(open(path, 'r'))
		for line in lines:
			if lines.indentation > len(stack):
				raise FormatError('unexpected indentation')
			stack = stack[:lines.indentation]
			pair = line.split(None, 1)
			stack.append(pair[0])
			key = '.'.join(stack).lower()
			if key not in self.handlers:
				raise FormatError('bad token: %s' % key)
			handler = self.handlers[key]
			if len(pair) == 1:
				if handler.constraint == self.needValue:
					raise FormatError('missing value')
				value = ''
			else:
				if not handler.callback or handler.constraint == self.noValue:
					raise FormatError('unexpected value')
				value = pair[1]
			if handler.callback:
				handler.callback(value)

################################################################################
# basic resource

class Resource(object):
	def __init__(self):
		if not hasattr(self, 'FromFileSecond'):
			self.FromFileSecond = self.FromFile
			self.FromFile = self.FromFileFirst
		if not hasattr(self, 'ToFileSecond') and hasattr(self, 'ToFile'):
			self.ToFileSecond = self.ToFile
			self.ToFile = self.ToFileFirst
		self.path = self.absPath = None

	def FromFileFirst(self, absPath = None, path = None):
		if not absPath:
			assert self.absPath
			absPath = self.absPath
		self.FromFileSecond(absPath)
		self.SetPath(absPath, path)
		return self
	def ToFileFirst(self, absPath = None, path = None):
		if not absPath:
			assert self.absPath
			absPath = self.absPath
		self.ToFileSecond(absPath)
		self.SetPath(absPath, path)

	def SetPath(self, absPath, path = None):
		absPath = os.path.normcase(os.path.normpath(absPath))
		if path is None:
			if absPath != self.absPath:
				self.path = GetSourcePath(absPath)
		else:
			self.path = path
		self.absPath = absPath

################################################################################
# camera set resource

class CameraSet(Resource):
	# tracking methods
	noTracking = 0
	positionTracking = 1
	orientationTracking = 2

	class Camera:
		matrix = property(lambda self:
			TranslationMatrix(self.position) *
			RotationMatrix(self.orientation))
		invMatrix = property(lambda self:
			RotationMatrix(self.orientation.Inv()) *
			TranslationMatrix(-self.position))
		viewMatrix = invMatrix

		def __init__(self):
			self.position = Vector3f()
			self.orientation = Quaternion()
			self.fov = 75
			self.tracking = CameraSet.noTracking
			self.trackingDistance = 0

		def GetProjMatrix(self, aspect):
			return ProjMatrix(aspect, DegToRad(self.fov))

	class TrackFace:
		def __init__(self):
			self.cameras = []

	def __init__(self):
		Resource.__init__(self)
		self.cameras = []
		self.trackFaces = []

	def FromFile(self, path):
		self.__init__()
		file = open(path, 'rb')
		if file.read(7) != 'PAGEcam':
			raise FormatError(path + ': invalid signature')
		cameras, trackFaces = struct.unpack('<2I', file.read(8))
		for i in range(cameras):
			camera = self.Camera()
			camera.position = Vector3f(struct.unpack('<3f', file.read(12)))
			camera.orientation = Quaternion(struct.unpack('<4f', file.read(16)))
			camera.fov, camera.tracking, camera.trackingDistance = \
				struct.unpack('<fIf', file.read(12))
			self.cameras.append(camera)
		for i in range(trackFaces):
			cameras, = struct.unpack('<I', file.read(4))
			trackFace = self.TrackFace()
			for j in range(cameras):
				camera, = struct.unpack('<I', file.read(4))
				trackFace.cameras.append(self.cameras[camera])
			self.trackFaces.append(trackFace)
		# FIXME: if available, load track geometry
		return self
	def ToFile(self, path):
		file = open(path, 'wb')
		file.write('PAGEcam')
		file.write(struct.pack('<2I', len(self.cameras), len(self.trackFaces)))
		for camera in self.cameras:
			file.write(struct.pack('<8fIf',
				camera.position[0], camera.position[1], camera.position[2],
				camera.orientation[0], camera.orientation[1], camera.orientation[2], camera.orientation[3],
				camera.fov, camera.tracking, camera.trackingDistance))
		for trackFace in self.trackFaces:
			file.write(struct.pack('<I', len(trackFace.cameras)))
			for camera in trackFace.cameras:
				file.write(struct.pack('<I', self.cameras.index(camera)))
		# FIXME: if available, save track geometry

################################################################################
# character resource

class Character(Resource):
	def __init__(self):
		Resource.__init__(self)
		self.name = ''
		self.gait = None
		self.model = None
		self.radius = 0
		self.scale = 1

	# parser callbacks
	def OnGait(self, value):
		self.gait = LoadResource(Gait, value)
	def OnModel(self, value):
		self.model = LoadResource(Model, value)
	def OnName(self, value):
		self.name = value
	def OnRadius(self, value):
		self.radius = float(value)
	def OnScale(self, value):
		self.scale = float(value)

	def FromFile(self, path):
		self.__init__()
		# FIXME: check signature
		parser = Parser()
		parser.Register('gait',   self.OnGait,   parser.needValue)
		parser.Register('model',  self.OnModel,  parser.needValue)
		parser.Register('name',   self.OnName,   parser.needValue)
		parser.Register('radius', self.OnRadius, parser.needValue)
		parser.Register('scale',  self.OnScale,  parser.needValue)
		parser.Parse(path)
		return self

################################################################################
# gait resource

class Gait(Resource):
	def FromFile(self, path):
		# FIXME: implement
		return self

################################################################################
# image resource

class Image(Resource):
	def __init__(self):
		Resource.__init__(self)
		self.handle = None

	def FromFile(self, path):
		self.handle = MakeTexture(path)
		return self

################################################################################
# material resource

class Material(Resource):
	class Pass:
		class Texture:
			def __init__(self):
				self.texture = None
				self.offset = Vector2f()
				self.scale = Vector2f(1)

		class Power:
			def __init__(self):
				self.value = 0
				self.texture = Material.Pass.Texture()

		class Ambient:
			def __init__(self):
				self.color = RgbColorf()
				self.texture = Material.Pass.Texture()

		class Diffuse:
			def __init__(self):
				self.color = RgbColorf(1)
				self.texture = Material.Pass.Texture()

		class Emissive:
			def __init__(self):
				self.color = RgbColorf()
				self.texture = Material.Pass.Texture()

		class Fresnel:
			def __init__(self):
				self.color = RgbColorf(1)
				self.texture = Material.Pass.Texture()
				self.power = Material.Pass.Power()

		class Gloss:
			def __init__(self):
				self.value = 0
				self.texture = Material.Pass.Texture()

		class Mask:
			def __init__(self):
				self.value = 1.
				self.texture = Material.Pass.Texture()

		class Normal:
			def __init__(self):
				self.texture = Material.Pass.Texture()

		class Specular:
			def __init__(self):
				self.color = RgbColorf()
				self.texture = Material.Pass.Texture()
				self.power = Material.Pass.Power()

		def __init__(self):
			self.ambient  = Material.Pass.Ambient()
			self.diffuse  = Material.Pass.Diffuse()
			self.emissive = Material.Pass.Emissive()
			self.fresnel  = Material.Pass.Fresnel()
			self.gloss    = Material.Pass.Gloss()
			self.mask     = Material.Pass.Mask()
			self.normal   = Material.Pass.Normal()
			self.specular = Material.Pass.Specular()

	def __init__(self):
		Resource.__init__(self)
		self.passes = []

	# parser callbacks
	def OnPass(self, value):
		self.passes.append(self.Pass())
	def OnPassAmbientColor(self, value):
		self.passes[-1].ambient.color = RgbColor([float(c) for c in value.split(None)])
	def OnPassAmbientTexture(self, value):
		if value: self.OnPassAmbientTextureImage(value)
	def OnPassAmbientTextureImage(self, value):
		self.passes[-1].ambient.texture.texture = LoadResource(Image, value)
	def OnPassAmbientTextureOffset(self, value):
		self.passes[-1].ambient.texture.offset = Vector2f(0).fromtextformat(value)
	def OnPassAmbientTextureScale(self, value):
		self.passes[-1].ambient.texture.scale = Vector2f(0).fromtextformat(value)
	def OnPassAmbientTextureUv(self, value):
		self.passes[-1].ambient.texture.uv = int(value)
	def OnPassDiffuseColor(self, value):
		self.passes[-1].diffuse.color = RgbColor([float(c) for c in value.split(None)])
	def OnPassDiffuseTexture(self, value):
		if value: self.OnPassDiffuseTextureImage(value)
	def OnPassDiffuseTextureImage(self, value):
		self.passes[-1].diffuse.texture.texture = LoadResource(Image, value)
	def OnPassDiffuseTextureOffset(self, value):
		self.passes[-1].diffuse.texture.offset = Vector2f(0).fromtextformat(value)
	def OnPassDiffuseTextureScale(self, value):
		self.passes[-1].diffuse.texture.scale = Vector2f(0).fromtextformat(value)
	def OnPassDiffuseTextureUv(self, value):
		self.passes[-1].diffuse.texture.uv = int(value)
	def OnPassEmissiveColor(self, value):
		self.passes[-1].emissive.color = RgbColor([float(c) for c in value.split(None)])
	def OnPassEmissiveTexture(self, value):
		if value: self.OnPassEmissiveTextureImage(value)
	def OnPassEmissiveTextureImage(self, value):
		self.passes[-1].emissive.texture.texture = LoadResource(Image, value)
	def OnPassEmissiveTextureOffset(self, value):
		self.passes[-1].emissive.texture.offset = Vector2f(0).fromtextformat(value)
	def OnPassEmissiveTextureScale(self, value):
		self.passes[-1].emissive.texture.scale = Vector2f(0).fromtextformat(value)
	def OnPassEmissiveTextureUv(self, value):
		self.passes[-1].emissive.texture.uv = int(value)
	def OnPassFresnelColor(self, value):
		self.passes[-1].fresnel.color = RgbColor([float(c) for c in value.split(None)])
	def OnPassFresnelPowerTexture(self, value):
		if value: self.OnPassFresnelPowerTextureImage(value)
	def OnPassFresnelPowerTextureImage(self, value):
		self.passes[-1].fresnel.power.texture.texture = LoadResource(Image, value)
	def OnPassFresnelPowerTextureOffset(self, value):
		self.passes[-1].fresnel.power.texture.offset = Vector2f(0).fromtextformat(value)
	def OnPassFresnelPowerTextureScale(self, value):
		self.passes[-1].fresnel.power.texture.scale = Vector2f(0).fromtextformat(value)
	def OnPassFresnelPowerTextureUv(self, value):
		self.passes[-1].fresnel.power.texture.uv = int(value)
	def OnPassFresnelPowerValue(self, value):
		self.passes[-1].fresnel.power.value = float(value)
	def OnPassFresnelTexture(self, value):
		if value: self.OnPassFresnelTextureImage(value)
	def OnPassFresnelTextureImage(self, value):
		self.passes[-1].fresnel.texture.texture = LoadResource(Image, value)
	def OnPassFresnelTextureOffset(self, value):
		self.passes[-1].fresnel.texture.offset = Vector2f(0).fromtextformat(value)
	def OnPassFresnelTextureScale(self, value):
		self.passes[-1].fresnel.texture.scale = Vector2f(0).fromtextformat(value)
	def OnPassFresnelTextureUv(self, value):
		self.passes[-1].fresnel.texture.uv = int(value)
	def OnPassGlossTexture(self, value):
		if value: self.OnPassGlossTextureImage(value)
	def OnPassGlossTextureImage(self, value):
		self.passes[-1].gloss.texture.texture = LoadResource(Image, value)
	def OnPassGlossTextureOffset(self, value):
		self.passes[-1].gloss.texture.offset = Vector2f(0).fromtextformat(value)
	def OnPassGlossTextureScale(self, value):
		self.passes[-1].gloss.texture.scale = Vector2f(0).fromtextformat(value)
	def OnPassGlossTextureUv(self, value):
		self.passes[-1].gloss.texture.uv = int(value)
	def OnPassGlossValue(self, value):
		self.passes[-1].gloss.value = float(value)
	def OnPassMaskTexture(self, value):
		if value: self.OnPassMaskTextureImage(value)
	def OnPassMaskTextureImage(self, value):
		self.passes[-1].mask.texture.texture = LoadResource(Image, value)
	def OnPassMaskTextureOffset(self, value):
		self.passes[-1].mask.texture.offset = Vector2f(0).fromtextformat(value)
	def OnPassMaskTextureScale(self, value):
		self.passes[-1].mask.texture.scale = Vector2f(0).fromtextformat(value)
	def OnPassMaskTextureUv(self, value):
		self.passes[-1].mask.texture.uv = int(value)
	def OnPassMaskValue(self, value):
		self.passes[-1].mask.value = float(value)
	def OnPassNormalTexture(self, value):
		if value: self.OnPassNormalTextureImage(value)
	def OnPassNormalTextureImage(self, value):
		self.passes[-1].normal.texture.texture = LoadResource(Image, value)
	def OnPassNormalTextureOffset(self, value):
		self.passes[-1].normal.texture.offset = Vector2f(0).fromtextformat(value)
	def OnPassNormalTextureScale(self, value):
		self.passes[-1].normal.texture.scale = Vector2f(0).fromtextformat(value)
	def OnPassNormalTextureUv(self, value):
		self.passes[-1].normal.texture.uv = int(value)
	def OnPassSpecularColor(self, value):
		self.passes[-1].specular.color = RgbColor([float(c) for c in value.split(None)])
	def OnPassSpecularPowerTexture(self, value):
		if value: self.OnPassSpecularPowerTextureImage(value)
	def OnPassSpecularPowerTextureImage(self, value):
		self.passes[-1].specular.power.texture.texture = LoadResource(Image, value)
	def OnPassSpecularPowerTextureOffset(self, value):
		self.passes[-1].specular.power.texture.offset = Vector2f(0).fromtextformat(value)
	def OnPassSpecularPowerTextureScale(self, value):
		self.passes[-1].specular.power.texture.scale = Vector2f(0).fromtextformat(value)
	def OnPassSpecularPowerTextureUv(self, value):
		self.passes[-1].specular.power.texture.uv = int(value)
	def OnPassSpecularPowerValue(self, value):
		self.passes[-1].specular.power.value = float(value)
	def OnPassSpecularTexture(self, value):
		if value: self.OnPassSpecularTextureImage(value)
	def OnPassSpecularTextureImage(self, value):
		self.passes[-1].specular.texture.texture = LoadResource(Image, value)
	def OnPassSpecularTextureOffset(self, value):
		self.passes[-1].specular.texture.offset = Vector2f(0).fromtextformat(value)
	def OnPassSpecularTextureScale(self, value):
		self.passes[-1].specular.texture.scale = Vector2f(0).fromtextformat(value)
	def OnPassSpecularTextureUv(self, value):
		self.passes[-1].specular.texture.uv = int(value)

	def FromFile(self, path):
		self.__init__()
		# FIXME: check signature
		parser = Parser()
		parser.Register('pass',                               self.OnPass,                           parser.noValue)
		parser.Register('pass.ambient')
		parser.Register('pass.ambient.color',                 self.OnPassAmbientColor,               parser.needValue)
		parser.Register('pass.ambient.texture',               self.OnPassAmbientTexture)
		parser.Register('pass.ambient.texture.image',         self.OnPassAmbientTextureImage,        parser.needValue)
		parser.Register('pass.ambient.texture.offset',        self.OnPassAmbientTextureOffset,       parser.needValue)
		parser.Register('pass.ambient.texture.scale',         self.OnPassAmbientTextureScale,        parser.needValue)
		parser.Register('pass.ambient.texture.uv',            self.OnPassAmbientTextureUv,           parser.needValue)
		parser.Register('pass.mask')
		parser.Register('pass.mask.texture',                  self.OnPassMaskTexture)
		parser.Register('pass.mask.texture.image',            self.OnPassMaskTextureImage,           parser.needValue)
		parser.Register('pass.mask.texture.offset',           self.OnPassMaskTextureOffset,          parser.needValue)
		parser.Register('pass.mask.texture.scale',            self.OnPassMaskTextureScale,           parser.needValue)
		parser.Register('pass.mask.texture.uv',               self.OnPassMaskTextureUv,              parser.needValue)
		parser.Register('pass.mask.value',                    self.OnPassMaskValue,                  parser.needValue)
		parser.Register('pass.diffuse')
		parser.Register('pass.diffuse.color',                 self.OnPassDiffuseColor,               parser.needValue)
		parser.Register('pass.diffuse.texture',               self.OnPassDiffuseTexture)
		parser.Register('pass.diffuse.texture.image',         self.OnPassDiffuseTextureImage,        parser.needValue)
		parser.Register('pass.diffuse.texture.offset',        self.OnPassDiffuseTextureOffset,       parser.needValue)
		parser.Register('pass.diffuse.texture.scale',         self.OnPassDiffuseTextureScale,        parser.needValue)
		parser.Register('pass.diffuse.texture.uv',            self.OnPassDiffuseTextureUv,           parser.needValue)
		parser.Register('pass.emissive')
		parser.Register('pass.emissive.color',                self.OnPassEmissiveColor,              parser.needValue)
		parser.Register('pass.emissive.texture',              self.OnPassEmissiveTexture)
		parser.Register('pass.emissive.texture.image',        self.OnPassEmissiveTextureImage,       parser.needValue)
		parser.Register('pass.emissive.texture.offset',       self.OnPassEmissiveTextureOffset,      parser.needValue)
		parser.Register('pass.emissive.texture.scale',        self.OnPassEmissiveTextureScale,       parser.needValue)
		parser.Register('pass.emissive.texture.uv',           self.OnPassEmissiveTextureUv,          parser.needValue)
		parser.Register('pass.fresnel')
		parser.Register('pass.fresnel.color',                 self.OnPassFresnelColor,               parser.needValue)
		parser.Register('pass.fresnel.power')
		parser.Register('pass.fresnel.power.texture',         self.OnPassFresnelPowerTexture)
		parser.Register('pass.fresnel.power.texture.image',   self.OnPassFresnelPowerTextureImage,   parser.needValue)
		parser.Register('pass.fresnel.power.texture.offset',  self.OnPassFresnelPowerTextureOffset,  parser.needValue)
		parser.Register('pass.fresnel.power.texture.scale',   self.OnPassFresnelPowerTextureScale,   parser.needValue)
		parser.Register('pass.fresnel.power.texture.uv',      self.OnPassFresnelPowerTextureUv,      parser.needValue)
		parser.Register('pass.fresnel.power.value',           self.OnPassFresnelPowerValue,          parser.needValue)
		parser.Register('pass.fresnel.texture',               self.OnPassFresnelTexture)
		parser.Register('pass.fresnel.texture.image',         self.OnPassFresnelTextureImage,        parser.needValue)
		parser.Register('pass.fresnel.texture.offset',        self.OnPassFresnelTextureOffset,       parser.needValue)
		parser.Register('pass.fresnel.texture.scale',         self.OnPassFresnelTextureScale,        parser.needValue)
		parser.Register('pass.fresnel.texture.uv',            self.OnPassFresnelTextureUv,           parser.needValue)
		parser.Register('pass.gloss')
		parser.Register('pass.gloss.texture',                 self.OnPassGlossTexture)
		parser.Register('pass.gloss.texture.image',           self.OnPassGlossTextureImage,          parser.needValue)
		parser.Register('pass.gloss.texture.offset',          self.OnPassGlossTextureOffset,         parser.needValue)
		parser.Register('pass.gloss.texture.scale',           self.OnPassGlossTextureScale,          parser.needValue)
		parser.Register('pass.gloss.texture.uv',              self.OnPassGlossTextureUv,             parser.needValue)
		parser.Register('pass.gloss.value',                   self.OnPassGlossValue,                 parser.needValue)
		parser.Register('pass.normal')
		parser.Register('pass.normal.texture',                self.OnPassNormalTexture)
		parser.Register('pass.normal.texture.image',          self.OnPassNormalTextureImage,         parser.needValue)
		parser.Register('pass.normal.texture.offset',         self.OnPassNormalTextureOffset,        parser.needValue)
		parser.Register('pass.normal.texture.scale',          self.OnPassNormalTextureScale,         parser.needValue)
		parser.Register('pass.normal.texture.uv',             self.OnPassNormalTextureUv,            parser.needValue)
		parser.Register('pass.specular')
		parser.Register('pass.specular.color',                self.OnPassSpecularColor,              parser.needValue)
		parser.Register('pass.specular.power')
		parser.Register('pass.specular.power.texture',        self.OnPassSpecularPowerTexture)
		parser.Register('pass.specular.power.texture.image',  self.OnPassSpecularPowerTextureImage,  parser.needValue)
		parser.Register('pass.specular.power.texture.offset', self.OnPassSpecularPowerTextureOffset, parser.needValue)
		parser.Register('pass.specular.power.texture.scale',  self.OnPassSpecularPowerTextureScale,  parser.needValue)
		parser.Register('pass.specular.power.texture.uv',     self.OnPassSpecularPowerTextureUv,     parser.needValue)
		parser.Register('pass.specular.power.value',          self.OnPassSpecularPowerValue,         parser.needValue)
		parser.Register('pass.specular.texture',              self.OnPassSpecularTexture)
		parser.Register('pass.specular.texture.image',        self.OnPassSpecularTextureImage,       parser.needValue)
		parser.Register('pass.specular.texture.offset',       self.OnPassSpecularTextureOffset,      parser.needValue)
		parser.Register('pass.specular.texture.scale',        self.OnPassSpecularTextureScale,       parser.needValue)
		parser.Register('pass.specular.texture.uv',           self.OnPassSpecularTextureUv,          parser.needValue)
		parser.Parse(path)
		return self

################################################################################
# mesh resource

class Mesh(Resource):
	class Face:
		def __init__(self, indices = None):
			self.indices = indices or []

	class Vertex:
		def __init__(self, co = None, no = None, uv = None, influences = None):
			self.co = Vector3f(co)
			self.no = Vector3f(no)
			self.uv = Vector2f(uv)
			self.influences = influences or []

	def __init__(self):
		Resource.__init__(self)
		self.faces = []
		self.vertices = []
		self.drawable = None

	def FromFile(self, path):
		self.__init__()
		file = open(path, 'rb')
		if file.read(8) != 'PAGEmesh':
			raise FormatError(path + ': invalid signature')
		numFaces, numVertices, numInfluences, numInfluenceNameIndices = \
			struct.unpack('<4I', file.read(16))
		for i in range(numFaces):
			self.faces.append(self.Face(struct.unpack('<3I', file.read(12))))
		for i in range(numVertices):
			data = struct.unpack('<8f2I', file.read(40))
			self.vertices.append(self.Vertex(data[0:3], data[3:6], data[6:8]))
		# FIXME: load influences
		return self

	def GetDrawable(self):
		if not self.drawable:
			self.drawable = VertexArray(self)
			self.dirty = False
		elif self.dirty:
			self.drawable.Update(self)
			self.dirty = False
		return self.drawable

################################################################################
# model resource

class Model(Resource):
	class Part:
		def __init__(self, name = None):
			self.name = name or ''
			self.mesh = None
			self.material = None

	def __init__(self):
		Resource.__init__(self)
		self.parts = []
		self.skeleton = None

	# parser callbacks
	def OnPart(self, value):
		self.parts.append(self.Part(value))
	def OnPartMaterial(self, value):
		self.parts[-1].material = LoadResource(Material, value)
	def OnPartMesh(self, value):
		self.parts[-1].mesh = LoadResource(Mesh, value)
	def OnSkeleton(self, value):
		self.skeleton = LoadResource(Skeleton, value)

	def FromFile(self, path):
		self.__init__()
		# FIXME: check signature
		parser = Parser()
		parser.Register('part',          self.OnPart)
		parser.Register('part.material', self.OnPartMaterial, parser.needValue)
		parser.Register('part.mesh',     self.OnPartMesh,     parser.needValue)
		parser.Register('skeleton',      self.OnSkeleton,     parser.needValue)
		parser.Parse(path)
		return self

################################################################################
# object resource

class Object(Resource):
	def __init__(self):
		Resource.__init__(self)
		self.name = ''
		self.model = None
		self.radius = 0
		self.scale = 1

	# parser callbacks
	def OnModel(self, value):
		self.model = LoadResource(Model, value)
	def OnName(self, value):
		self.name = value
	def OnRadius(self, value):
		self.radius = float(value)
	def OnScale(self, value):
		self.scale = float(value)

	def FromFile(self, path):
		self.__init__()
		# FIXME: check signature
		parser = Parser()
		parser.Register('model',  self.OnModel,  parser.needValue)
		parser.Register('name',   self.OnName,   parser.needValue)
		parser.Register('radius', self.OnRadius, parser.needValue)
		parser.Register('scale',  self.OnScale,  parser.needValue)
		parser.Parse(path)
		return self

################################################################################
# scene resource

class Scene(Resource):
	class Form(PositionOrientationScale):
		def __init__(self, name = None, model = None):
			PositionOrientationScale.__init__(self)
			self.name = name or ''
			self.model = model

	def __init__(self):
		Resource.__init__(self)
		self.cameraSet = None
		self.forms = []
		self.music = None
		self.script = None
		self.track = None

	# parser callbacks
	def OnCameraSet(self, value):
		self.cameraSet = LoadResource(CameraSet, value)
	def OnForm(self, value):
		self.forms.append(self.Form(value))
	def OnFormModel(self, value):
		self.forms[-1].model = LoadResource(Model, value)
	def OnFormOrientation(self, value):
		self.forms[-1].orientation = Euler(Quaternion([float(component) for component in value.split()]))
	def OnFormPosition(self, value):
		self.forms[-1].position = Vector3f([float(component) for component in value.split()])
	def OnFormScale(self, value):
		self.forms[-1].scale = Vector3f([float(component) for component in value.split()])
	def OnMusic(self, value):
		self.music = LoadResource(Sound, value)
	def OnScript(self, value):
		self.script = LoadResource(Script, value)
	def OnTrack(self, value):
		self.track = LoadResource(Track, value)

	def FromFile(self, path):
		self.__init__()
		# FIXME: check signature
		parser = Parser()
		parser.Register('cameraset',        self.OnCameraSet,       parser.needValue)
		parser.Register('form',             self.OnForm)
		parser.Register('form.model',       self.OnFormModel,       parser.needValue)
		parser.Register('form.orientation', self.OnFormOrientation, parser.needValue)
		parser.Register('form.position',    self.OnFormPosition,    parser.needValue)
		parser.Register('form.scale',       self.OnFormScale,       parser.needValue)
		parser.Register('music',            self.OnMusic,           parser.needValue)
		parser.Register('script',           self.OnScript,          parser.needValue)
		parser.Register('track',            self.OnTrack,           parser.needValue)
		parser.Parse(path)
		return self
	def ToFile(self, path):
		file = open(path, 'w')
		file.write('#!PAGEscene\n')
		for form in self.forms:
			file.write('form%s\n' % (form.name and ' ' + form.name or ''))
			if form.model and form.model.path:
				file.write('\tmodel %s\n' % form.model.path)
				if form.position != Vector3f():
					file.write('\tposition %s\n' % form.position.totextformat())
				if form.orientation != Euler():
					file.write('\torientation %s\n' % Quaternion(form.orientation).totextformat())
				if form.scale != Vector3f(1, 1, 1):
					file.write('\tscale %s\n' % form.scale.totextformat())
		if self.cameraSet and self.cameraSet.path:
			file.write('cameraset %s\n' % self.cameraSet.path)
		if self.music and self.music.path:
			file.write('music %s\n' % self.music.path)
		if self.script and self.script.path:
			file.write('script %s\n' % self.script.path)
		if self.track and self.track.path:
			file.write('track %s\n' % self.track.path)

################################################################################
# script resource

class Script(Resource):
	def FromFile(self, path):
		# FIXME: implement
		return self

################################################################################
# skeleton resource

class Skeleton(Resource):
	def FromFile(self, path):
		# FIXME: implement
		return self

################################################################################
# sound resource

class Sound(Resource):
	def FromFile(self, path):
		# FIXME: implement
		return self

################################################################################
# track resource

class Track(Resource):
	class Face:
		def __init__(self, vertices = None, neighbours = None):
			self.vertices = vertices or []
			self.neighbours = neighbours or []

	def __init__(self):
		Resource.__init__(self)
		self.faces = []
		self.drawable = None
		self.selectDrawable = None

	def FromFile(self, path):
		self.__init__()
		file = open(path, 'rb')
		if file.read(9) != 'PAGEtrack':
			raise FormatError(path + ': invalid signature')
		faces, = struct.unpack('<I', file.read(4))
		for face in range(faces):
			vertices = []
			for i in range(3):
				vertices.append(Vector3f(struct.unpack('<3f', file.read(12))))
			neighbours = struct.unpack('<3i', file.read(12))
			self.faces.append(self.Face(vertices, neighbours))
		return self

	def GetDrawable(self):
		if not self.drawable:
			self.drawable = VertexArray(self)
		return self.drawable
	def GetSelectDrawable(self):
		if not self.selectDrawable:
			self.selectDrawable = DisplayList().FromTrackWithNames(self)
		return self.selectDrawable
