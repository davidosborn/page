from __future__ import absolute_import

import math # tan
from .config import prefs # camera.fov
from .math import DegToRad, Euler, Matrix, ProjMatrix, RotationMatrix, ScaleMatrix, TranslationMatrix, Vector3f

################################################################################
# attributes

class Transformable(object):
	matrix               = property(lambda self: self.GetMatrix())
	invMatrix            = property(lambda self: self.GetInvMatrix())
	transformedMatrix    = property(lambda self: self.GetTransformedMatrix())
	invTransformedMatrix = property(lambda self: self.GetInvTransformedMatrix())

class Position(Transformable):
	transformedPosition = property(lambda self: self.position + self.translation)

	def __init__(self, position = None):
		if position is None: self.position = Vector3f()
		else: self.position = Vector3f(position)
		Position.ResetTransformation(self)

	def GetMatrix(self):
		return TranslationMatrix(self.position)
	def GetInvMatrix(self):
		return TranslationMatrix(-self.position)
	def GetTransformedMatrix(self):
		return TranslationMatrix(self.transformedPosition)
	def GetInvTransformedMatrix(self):
		return TranslationMatrix(-self.transformedPosition)

	def ResetTransformation(self):
		self.translation = Vector3f()
	def BakeTransformation(self):
		self.position = self.transformedPosition
		Position.ResetTransformation(self)

class Orientation(object):
	transformedOrientation = property(lambda self: self.orientation * self.rotation)

	def __init__(self, orientation = None):
		if orientation is None: self.orientation = Euler()
		else: self.orientation = Euler(orientation)
		Orientation.ResetTransformation(self)

	def GetMatrix(self):
		return RotationMatrix(self.orientation)
	def GetInvMatrix(self):
		return RotationMatrix(self.orientation).Tpos()
	def GetTransformedMatrix(self):
		return RotationMatrix(self.transformedOrientation)
	def GetInvTransformedMatrix(self):
		return RotationMatrix(self.transformedOrientation).Tpos()

	def ResetTransformation(self):
		self.rotation = Euler()
	def BakeTransformation(self):
		self.orientation = self.transformedOrientation
		Orientation.ResetTransformation(self)

class Scale(object):
	transformedScale = property(lambda self: self.scale * self.rescale)

	def __init__(self, scale = None):
		if scale is None: self.scale = Vector3f(1)
		else: self.scale = Vector3f(scale)
		Scale.ResetTransformation(self)

	def GetMatrix(self):
		return ScaleMatrix(self.scale)
	def GetInvMatrix(self):
		return ScaleMatrix(1 / self.scale)
	def GetTransformedMatrix(self):
		return ScaleMatrix(self.transformedScale)
	def GetInvTransformedMatrix(self):
		return ScaleMatrix(1 / self.transformedScale)

	def ResetTransformation(self):
		self.rescale = Vector3f(1)
	def BakeTransformation(self):
		self.scale = self.transformedScale
		Scale.ResetTransformation(self)

class PositionOrientation(Position, Orientation):
	def __init__(self, position = None, orientation = None):
		Position.__init__(self, position)
		Orientation.__init__(self, orientation)

	def GetMatrix(self):
		return (
			Position.GetMatrix(self) *
			Orientation.GetMatrix(self))
	def GetInvMatrix(self):
		return (
			Orientation.GetInvMatrix(self) *
			Position.GetInvMatrix(self))
	def GetTransformedMatrix(self):
		return (
			Position.GetTransformedMatrix(self) *
			Orientation.GetTransformedMatrix(self))
	def GetInvTransformedMatrix(self):
		return (
			Orientation.GetInvTransformedMatrix(self) *
			Position.GetInvTransformedMatrix(self))

	def ResetTransformation(self):
		Position.ResetTransformation(self)
		Orientation.ResetTransformation(self)
	def BakeTransformation(self):
		Position.BakeTransformation(self)
		Orientation.BakeTransformation(self)

class PositionOrientationScale(PositionOrientation, Scale):
	def __init__(self, position = None, orientation = None, scale = None):
		PositionOrientation.__init__(self, position, orientation)
		Scale.__init__(self, scale)

	def GetMatrix(self):
		return (
			PositionOrientation.GetMatrix(self) *
			Scale.GetMatrix(self))
	def GetInvMatrix(self):
		return (
			Scale.GetInvMatrix(self) *
			PositionOrientation.GetInvMatrix(self))
	def GetTransformedMatrix(self):
		return (
			PositionOrientation.GetTransformedMatrix(self) *
			Scale.GetTransformedMatrix(self))
	def GetInvTransformedMatrix(self):
		return (
			Scale.GetInvTransformedMatrix(self) *
			PositionOrientation.GetInvTransformedMatrix(self))

	def ResetTransformation(self):
		PositionOrientation.ResetTransformation(self)
		Scale.ResetTransformation(self)
	def BakeTransformation(self):
		PositionOrientation.BakeTransformation(self)
		Scale.BakeTransformation(self)

################################################################################
# camera

class Camera(PositionOrientation):
	viewMatrix = property(lambda self: self.GetViewMatrix())

	def __init__(self, position = None, orientation = None):
		PositionOrientation.__init__(self, position, orientation)
		self.fov = prefs['camera.fov']

	def GetProjMatrix(self, aspect):
		return ProjMatrix(aspect, DegToRad(self.fov))
	def GetViewMatrix(self):
		return self.invMatrix
