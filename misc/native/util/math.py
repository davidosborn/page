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

import array # array
import itertools # repeat
import math # a{sin,tan2}, cos, pi, sin, sqrt
import operator
from .type_traits import CopyType, IsArithmeticType, IsIterable

PosInf = 1e300000
NegInf = -PosInf

################################################################################
# angle conversions

def DegToRad(x): return x * math.pi / 180
def DegToRev(x): return x / 360.
def RadToDeg(x): return x * 180 / math.pi
def RadToRev(x): return x / (math.pi * 2)
def RevToDeg(x): return x * 360.
def RevToRad(x): return x * (math.pi * 2)

################################################################################
# base for array primitives

class BasicArray(object):
	def __init__(self, components, sliceFunction = None):
		self._components = components
		self._sliceFunction = sliceFunction

	# container operators
	def __len__(self):
		return len(self._components)
	def __getitem__(self, key):
		if isinstance(key, slice):
			return type(self)(self._components[key])
		return self._components[key]
	def __setitem__(self, i, value):
		self._components[i] = CopyType(value, self._components[i])
	def __iter__(self):
		return iter(self._components)
	def __contains__(self, other):
		return other in self._components

	# comparison operators
	def __eq__(self, other): return self._components == other._components
	def __ne__(self, other): return self._components != other._components
	def __nonzero__(self): return any(self._components)

	# serialization operators
	def __str__(self):
		return ','.join([str(component) for component in self._components])
	def __repr__(self):
		return ','.join([repr(component) for component in self._components])

	# storage serialization
	def toarray(self, typecode = None):
		if not typecode:
			if   isinstance(self[0], complex): typecode = 'd'
			elif isinstance(self[0], int):     typecode = 'i'
			elif isinstance(self[0], long):    typecode = 'l'
			else:                              typecode = 'f'
		return array.array(typecode, self._components)
	def tofile(self, file):
		file.write(self.tostring())
	def tolist(self):
		return self._components
	def tostring(self):
		return self.toarray().tostring()
	def totextformat(self):
		return ' '.join([str(component) for component in self._components])

	# conversion deserialization
	def fromtextformat(self, string):
		strings = string.split()
		if len(strings) == 1:
			string = strings[0]
			for component in self._components:
				component = type(component)(string)
		else:
			assert len(strings) == len(self._components)
			for component, string in zip(self._components, strings):
				component = type(component)(string)
		return self

################################################################################
# vector

class Vector(BasicArray):
	x = property(lambda self: self[0], lambda self, value: self.__setitem__(0, value))
	y = property(lambda self: self[1], lambda self, value: self.__setitem__(1, value))
	z = property(lambda self: self[2], lambda self, value: self.__setitem__(2, value))
	w = property(lambda self: self[3], lambda self, value: self.__setitem__(3, value))

	def __init__(self, *args):
		if len(args) == 1 and IsIterable(args[0]):
			args = tuple(args[0])
		assert len(args)
		assert IsArithmeticType(args[0])
		coerced = args[0]
		for arg in args[1:]:
			coerced = coerce(arg, coerced)[0]
		BasicArray.__init__(self, [coerce(arg, coerced)[0] for arg in args])

	# storage adapter
	def _MakeCompatible(self, other):
		if not isinstance(other, Vector):
			if IsIterable(other): other = Vector(other)
			else: other = Vector(itertools.repeat(other, len(self)))
		assert len(self) == len(other)
		return other

	# coercion operators
	def __coerce__(self, other):
		coerced = [coerce(a, b) for a, b in zip(self, self._MakeCompatible(other))]
		return (
			Vector([pair[0] for pair in coerced]),
			Vector([pair[1] for pair in coerced]))
	def __complex__(self): return Vector([complex(c) for c in self])
	def __float__  (self): return Vector([float  (c) for c in self])
	def __int__    (self): return Vector([int    (c) for c in self])
	def __long__   (self): return Vector([long   (c) for c in self])

	# unary arithmetic operators
	def __abs__   (self): return Vector([operator.abs(c) for c in self])
	def __invert__(self): return Vector([operator.inv(c) for c in self])
	def __neg__   (self): return Vector([operator.neg(c) for c in self])
	def __pos__   (self): return Vector([operator.pos(c) for c in self])

	# binary operator helpers
	def _BinaryOp(self, other, op):
		return Vector([op(a, b) for a, b in zip(self, self._MakeCompatible(other))])
	def _RevBinaryOp(self, other, op):
		return Vector([op(b, a) for a, b in zip(self, self._MakeCompatible(other))])

	# binary arithmetic operators
	def __add__     (self, other): return self._BinaryOp(other, operator.add)
	def __and__     (self, other): return self._BinaryOp(other, operator.and_)
	def __div__     (self, other): return self._BinaryOp(other, operator.div)
	def __divmod__  (self, other): return self._BinaryOp(other, operator.divmod)
	def __floordiv__(self, other): return self._BinaryOp(other, operator.floordiv)
	def __lshift__  (self, other): return self._BinaryOp(other, operator.lshift)
	def __mod__     (self, other): return self._BinaryOp(other, operator.mod)
	def __mul__     (self, other): return self._BinaryOp(other, operator.mul)
	def __or__      (self, other): return self._BinaryOp(other, operator.or_)
	def __pow__     (self, other): return self._BinaryOp(other, operator.pow)
	def __rshift__  (self, other): return self._BinaryOp(other, operator.rshift)
	def __sub__     (self, other): return self._BinaryOp(other, operator.sub)
	def __truediv__ (self, other): return self._BinaryOp(other, operator.truediv)
	def __xor__     (self, other): return self._BinaryOp(other, operator.xor)

	# reverse binary arithmetic operators
	def __radd__     (self, other): return self._RevBinaryOp(other, operator.add)
	def __rand__     (self, other): return self._RevBinaryOp(other, operator.and_)
	def __rdiv__     (self, other): return self._RevBinaryOp(other, operator.div)
	def __rdivmod__  (self, other): return self._RevBinaryOp(other, operator.divmod)
	def __rfloordiv__(self, other): return self._RevBinaryOp(other, operator.floordiv)
	def __rlshift__  (self, other): return self._RevBinaryOp(other, operator.lshift)
	def __rmod__     (self, other): return self._RevBinaryOp(other, operator.mod)
	def __rmul__     (self, other): return self._RevBinaryOp(other, operator.mul)
	def __ror__      (self, other): return self._RevBinaryOp(other, operator.or_)
	def __rpow__     (self, other): return self._RevBinaryOp(other, operator.pow)
	def __rrshift__  (self, other): return self._RevBinaryOp(other, operator.rshift)
	def __rsub__     (self, other): return self._RevBinaryOp(other, operator.sub)
	def __rtruediv__ (self, other): return self._RevBinaryOp(other, operator.truediv)
	def __rxor__     (self, other): return self._RevBinaryOp(other, operator.xor)

	# comparison operators
	def __eq__(self, other): return self._BinaryOp(other, operator.eq)
	def __ge__(self, other): return self._BinaryOp(other, operator.ge)
	def __gt__(self, other): return self._BinaryOp(other, operator.gt)
	def __le__(self, other): return self._BinaryOp(other, operator.le)
	def __lt__(self, other): return self._BinaryOp(other, operator.lt)
	def __ne__(self, other): return self._BinaryOp(other, operator.ne)

	def Angle(self, other):
		assert len(self) == len(other)
		if len(self) == 2:
			return math.atan2(self.PerpDot(other), self.Dot(other))
		assert len(self) == 3
		# algorithm courtesy of Jesse Anders
		# http://www.gamedev.net/community/forums/topic.asp?topic_id=359053&#2357351
		return math.atan2(self.Cross(other).Len(), self.Dot(other))

	def Cross(self, other = None):
		if len(self) == 2:
			assert other is None
			return Vector(-self.y, self.x)
		assert isinstance(other, Vector)
		assert len(self) == len(other) == 3
		return (
			self.Swizzle(1, 2, 0) * other.Swizzle(2, 0, 1) -
			self.Swizzle(2, 0, 1) * other.Swizzle(1, 2, 0))

	def Dot(self, other):
		return sum(self * other)

	def Len(self):
		return math.sqrt(self.Dot(self))

	def Norm(self):
		return self / self.Len()

	def PerpDot(self, other):
		assert len(self) == len(other) == 2
		return self.Dot(other.Cross())

	def Swizzle(self, *args):
		return Vector([self[i] for i in args])

# initialization
def MakeVector(dim, type, args):
	if len(args) == 1:
		if args[0] is None: args = ()
		elif IsIterable(args[0]): args = args[0]
	args = [type(arg) for arg in tuple(args)[:dim]]
	if len(args) < dim:
		if len(args) == 1: fill = args[0]
		else: fill = type(0)
		args += itertools.repeat(fill, dim - len(args))
		if dim == 4: args[-1] = type(1)
	return Vector(args)
def Vector2c(*args): return MakeVector(2, complex, args)
def Vector3c(*args): return MakeVector(3, complex, args)
def Vector4c(*args): return MakeVector(4, complex, args)
def Vector2f(*args): return MakeVector(2, float, args)
def Vector3f(*args): return MakeVector(3, float, args)
def Vector4f(*args): return MakeVector(4, float, args)
def Vector2i(*args): return MakeVector(2, int, args)
def Vector3i(*args): return MakeVector(3, int, args)
def Vector4i(*args): return MakeVector(4, int, args)
def Vector2l(*args): return MakeVector(2, long, args)
def Vector3l(*args): return MakeVector(3, long, args)
def Vector4l(*args): return MakeVector(4, long, args)

################################################################################
# euler

class Euler(BasicArray):
	yaw   = property(lambda self: self[0], lambda self, value: self.__setitem__(0, value))
	pitch = property(lambda self: self[1], lambda self, value: self.__setitem__(1, value))
	roll  = property(lambda self: self[2], lambda self, value: self.__setitem__(2, value))

	def __init__(self, *args):
		if len(args) == 1:
			if isinstance(args[0], Matrix):
				m = args[0]
				if m.yz > .999: # north pole singularity
					args = [
						math.atan2(m.xy, m.zy),
						math.pi / 2]
				elif m.yz < -.999: # south pole singularity
					args = [
						math.atan2(m.xy, m.zy),
						math.pi / 2]
				else:
					args = [
						math.atan2(m.xz, m.zz),
						math.asin(-m.yz),
						math.atan2(m.yx, m.yy)]
			elif isinstance(args[0], Quaternion):
				q = args[0]
				test = q.w * q.x - q.y * q.z
				if test > .4995: # north pole singularity
					args = [
						math.atan2(q.y, q.w) * 2,
						math.pi / 2]
				elif test < -.4995: # south pole singularity
					args = [
						-math.atan2(q.y, q.w) * 2,
						-math.pi / 2]
				else:
					args = [
						math.atan2((q.x * q.z + q.w * q.y) * 2, 1 - (q.x * q.x + q.y * q.y) * 2),
						math.asin(test * 2),
						math.atan2((q.x * q.y + q.w * q.z) * 2, 1 - (q.x * q.x + q.z * q.z) * 2)]
			elif args[0] is None: args = []
			elif IsIterable(args[0]): args = list(args[0])
		else: args = list(args)
		assert len(args) <= 3
		if len(args) < 3:
			args += itertools.repeat(0, 3 - len(args))
		BasicArray.__init__(self, [float(arg) for arg in args])

	# storage adapter
	def _MakeCompatible(self, other):
		if not isinstance(other, Euler):
			if IsIterable(other): other = Euler(other)
			else: other = Euler(itertools.repeat(other, 3))
		return other

	# unary arithmetic operators
	def __abs__   (self): return Euler([operator.abs(c) for c in self])
	def __neg__   (self): return Euler([operator.neg(c) for c in self])
	def __pos__   (self): return Euler([operator.pos(c) for c in self])

	# binary operator helpers
	def _BinaryOp(self, other, op):
		return Euler([op(a, b) for a, b in zip(self, self._MakeCompatible(other))])
	def _RevBinaryOp(self, other, op):
		return Euler([op(b, a) for a, b in zip(self, self._MakeCompatible(other))])

	# binary arithmetic operators
	def __add__(self, other): return self._BinaryOp(other, operator.add)
	def __div__(self, other): return self._BinaryOp(other, operator.div)
	def __sub__(self, other): return self._BinaryOp(other, operator.sub)

	# reverse binary arithmetic operators
	def __radd__(self, other): return self._RevBinaryOp(other, operator.add)
	def __rdiv__(self, other): return self._RevBinaryOp(other, operator.div)
	def __rsub__(self, other): return self._RevBinaryOp(other, operator.sub)

	# concatenation/multiplication/transformation operators
	def __mul__(self, other):
		if isinstance(other, Euler):
			return self + other
		if isinstance(other, Matrix):
			return Euler(RotationMatrix(self) * other)
		if isinstance(other, Quaternion):
			return Euler(Quaternion(self) * other)
		if isinstance(other, Vector):
			return RotationMatrix(self) * other
		if IsArithmeticType(other):
			return Euler([operator.mul(c, other) for c in self])
		raise TypeError('unsupported operand')
	def __rmul__(self, other):
		if isinstance(other, Euler):
			return other + self
		if isinstance(other, Matrix):
			return Euler(other * RotationMatrix(self))
		if isinstance(other, Quaternion):
			return Euler(other * Quaternion(self))
		if IsArithmeticType(other):
			return Euler([operator.mul(other, c) for c in self])
		raise TypeError('unsupported operand')

def EulerBetween(a, b):
	assert isinstance(a, Vector) and isinstance(b, Vector)
	assert len(a) == len(b) == 3
	return Euler(0,
		a.Swizzle(0, 1).Angle(b.Swizzle(0, 1)),
		a.Swizzle(1, 2).Angle(b.Swizzle(1, 2)))

################################################################################
# quaternion

class Quaternion(BasicArray):
	x = property(lambda self: self[0], lambda self, value: self.__setitem__(0, value))
	y = property(lambda self: self[1], lambda self, value: self.__setitem__(1, value))
	z = property(lambda self: self[2], lambda self, value: self.__setitem__(2, value))
	w = property(lambda self: self[3], lambda self, value: self.__setitem__(3, value))

	def __init__(self, *args):
		if len(args) == 1:
			if isinstance(args[0], Euler):
				e = args[0]
				cy = math.cos(e.yaw   / 2)
				sy = math.sin(e.yaw   / 2)
				cp = math.cos(e.pitch / 2)
				sp = math.sin(e.pitch / 2)
				cr = math.cos(e.roll  / 2)
				sr = math.sin(e.roll  / 2)
				args = (
					cy * sp * cr + sy * cp * sr,
					sy * cp * cr - cy * sp * sr,
					cy * cp * sr - sy * sp * cr,
					cy * cp * cr + sy * sp * sr)
			elif isinstance(args[0], Matrix):
				m = args[0]
				trace = m.xx + m.yy + m.zz
				if trace > 0:
					s = .5 / math.sqrt(trace + 1)
					args = (
						(m.zy - m.yz) * s,
						(m.xz - m.zx) * s,
						(m.yx - m.xy) * s,
						.25 / s)
				elif m.xx > m.yy and m.xx > m.zz:
					s = .5 / math.sqrt(m.xx - m.yy - m.zz + 1)
					args = (
						.25 / s,
						(m.yx + m.xy) * s,
						(m.xz + m.zx) * s,
						(m.zy - m.yz) * s)
				elif m.yy > m.zz:
					s = .5 / math.sqrt(m.yy - m.zz - m.xx + 1)
					args = (
						(m.yx + m.xy) * s,
						.25 / s,
						(m.zy + m.yz) * s,
						(m.xz - m.zx) * s)
				else:
					s = .5 / math.sqrt(m.zz - m.xx - m.yy + 1)
					args = (
						(m.xz + m.zx) * s,
						(m.zy + m.yz) * s,
						.25 / s,
						(m.yx - m.xy) * s)
			elif args[0] is None: args = ()
			elif IsIterable(args[0]): args = tuple(args[0])
		if len(args) == 0: args = (0, 0, 0, 1)
		assert len(args) == 4
		BasicArray.__init__(self, [float(arg) for arg in args])

	# unary arithmetic operators
	def __abs__(self): return Quaternion([operator.abs(c) for c in self])
	def __neg__(self): return Quaternion([operator.neg(c) for c in self])
	def __pos__(self): return Quaternion([operator.pos(c) for c in self])

	# concatenation/transformation operators
	def __mul__(self, other):
		if isinstance(other, Vector):
			assert len(other) == 3 or len(other) == 4
			return Vector3f(self * Quaternion(Vector4f(other)) * self.Inv())
		if not isinstance(other, Quaternion):
			other = Quaternion(other)
		return Quaternion(
			self.w * other.x + self.x * other.w + self.y * other.z - self.z * other.y,
			self.w * other.y + self.y * other.w + self.z * other.x - self.x * other.z,
			self.w * other.z + self.z * other.w + self.x * other.y - self.y * other.x,
			self.w * other.w - self.x * other.x - self.y * other.y - self.z * other.z)
	def __rmul__(self, other):
		return Quaternion(other) * self

	def Inv(self):
		return Quaternion(-self.x, -self.y, -self.z, self.w)

################################################################################
# matrix

class Matrix(BasicArray):
	xx = property(lambda self: self[0],  lambda self, value: self.__setitem__(0,  value))
	yx = property(lambda self: self[1],  lambda self, value: self.__setitem__(1,  value))
	zx = property(lambda self: self[2],  lambda self, value: self.__setitem__(2,  value))
	wx = property(lambda self: self[3],  lambda self, value: self.__setitem__(3,  value))
	xy = property(lambda self: self[4],  lambda self, value: self.__setitem__(4,  value))
	yy = property(lambda self: self[5],  lambda self, value: self.__setitem__(5,  value))
	zy = property(lambda self: self[6],  lambda self, value: self.__setitem__(6,  value))
	wy = property(lambda self: self[7],  lambda self, value: self.__setitem__(7,  value))
	xz = property(lambda self: self[8],  lambda self, value: self.__setitem__(8,  value))
	yz = property(lambda self: self[9],  lambda self, value: self.__setitem__(9,  value))
	zz = property(lambda self: self[10], lambda self, value: self.__setitem__(10, value))
	wz = property(lambda self: self[11], lambda self, value: self.__setitem__(11, value))
	xw = property(lambda self: self[12], lambda self, value: self.__setitem__(12, value))
	yw = property(lambda self: self[13], lambda self, value: self.__setitem__(13, value))
	zw = property(lambda self: self[14], lambda self, value: self.__setitem__(14, value))
	ww = property(lambda self: self[15], lambda self, value: self.__setitem__(15, value))

	def __init__(self, *args):
		if len(args) == 1:
			if args[0] is None: args = ()
			elif IsIterable(args[0]): args = args[0]
		if len(args) == 0:
			args = (
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1)
		else:
			assert len(args) == 16
		BasicArray.__init__(self, [float(arg) for arg in args])

	# binary arithmetic operators
	def __mul__(self, other):
		if isinstance(other, Vector):
			if len(other) == 3:
				other = Vector4f(other)
				return Vector(
					self.Row(0).Dot(other),
					self.Row(1).Dot(other),
					self.Row(2).Dot(other))
			else:
				assert len(other) == 4
				return Vector(
					self.Row(0).Dot(other),
					self.Row(1).Dot(other),
					self.Row(2).Dot(other),
					self.Row(3).Dot(other))
		if isinstance(other, Euler) or isinstance(other, Quaternion):
			other = RotationMatrix(other)
		elif not isinstance(other, Matrix):
			other = Matrix(other)
		return Matrix(
			self.Row(0).Dot(other.Col(0)),
			self.Row(1).Dot(other.Col(0)),
			self.Row(2).Dot(other.Col(0)),
			self.Row(3).Dot(other.Col(0)),
			self.Row(0).Dot(other.Col(1)),
			self.Row(1).Dot(other.Col(1)),
			self.Row(2).Dot(other.Col(1)),
			self.Row(3).Dot(other.Col(1)),
			self.Row(0).Dot(other.Col(2)),
			self.Row(1).Dot(other.Col(2)),
			self.Row(2).Dot(other.Col(2)),
			self.Row(3).Dot(other.Col(2)),
			self.Row(0).Dot(other.Col(3)),
			self.Row(1).Dot(other.Col(3)),
			self.Row(2).Dot(other.Col(3)),
			self.Row(3).Dot(other.Col(3)))
	def __rmul__(self, other):
		if isinstance(other, Vector):
			if len(other) == 3: other = Vector4f(other)
			else: assert len(other) == 4
			return Vector(
				other.Dot(self.Col(0)),
				other.Dot(self.Col(1)),
				other.Dot(self.Col(2)),
				other.Dot(self.Col(3)))
		if isinstance(other, Euler) or isinstance(other, Quaternion):
			other = RotationMatrix(other)
		elif not isinstance(other, Matrix):
			other = Matrix(other)
		return other * self

	def Row(self, n): return Vector(self[n], self[n + 4], self[n + 8], self[n + 12])
	def Col(self, n): return Vector(self[n * 4], self[n * 4 + 1], self[n * 4 + 2], self[n * 4 + 3])

	def Tpos(self):
		return Matrix(
			self.xx, self.xy, self.xz, self.xw,
			self.yx, self.yy, self.yz, self.yw,
			self.zx, self.zy, self.zz, self.zw,
			self.wx, self.wy, self.wz, self.ww)

# initialization
def TranslationMatrix(translation):
	if not isinstance(translation, Vector):
		translation = Vector(translation)
	return Matrix(
		1, 0, 0, translation.x,
		0, 1, 0, translation.y,
		0, 0, 1, translation.z,
		0, 0, 0, 1).Tpos()
def RotationMatrix(rotation):
	if isinstance(rotation, Euler):
		cy = math.cos(rotation.yaw)
		sy = math.sin(rotation.yaw)
		cp = math.cos(rotation.pitch)
		sp = math.sin(rotation.pitch)
		cr = math.cos(rotation.roll)
		sr = math.sin(rotation.roll)
		return Matrix(
			sy * sp * sr + cy * cr,
			cp * sr,
			cy * sp * sr - sy * cr,
			0,
			sy * sp * cr - cy * sr,
			cp * cr,
			cy * sp * cr + sy * sr,
			0,
			sy * cp,
			-sp,
			cy * cp,
			0, 0, 0, 0, 1)
	if isinstance(rotation, Quaternion):
		x2 = rotation.x * 2;  y2 = rotation.y * 2;  z2 = rotation.z * 2
		wx = rotation.w * x2; wy = rotation.w * y2; wz = rotation.w * z2
		xx = rotation.x * x2; xy = rotation.x * y2; xz = rotation.x * z2
		yy = rotation.y * y2; yz = rotation.y * z2; zz = rotation.z * z2
		return Matrix(
			1 - yy - zz,
			xy + wz,
			xz - wy,
			0,
			xy - wz,
			1 - xx - zz,
			yz + wx,
			0,
			xz + wy,
			yz - wx,
			1 - xx - yy,
			0, 0, 0, 0, 1)
	raise TypeError
def ScaleMatrix(scale):
	if not isinstance(scale, Vector):
		scale = Vector(scale)
	return Matrix(
		scale.x, 0, 0, 0,
		0, scale.y, 0, 0,
		0, 0, scale.z, 0,
		0, 0, 0, 1)
def ProjMatrix(aspect, fov, near = .05, far = 500):
	d = 1 / math.tan(fov / 2.)
	return Matrix(
		d / aspect, 0, 0, 0,
		0, d, 0, 0,
		0, 0, (near + far) / float(near - far),
			near * far * 2 / float(near - far),
		0, 0, -1, 0).Tpos()

################################################################################
# color

class RgbColor(BasicArray):
	def __init__(self, *args):
		# FIXME: implement
		BasicArray.__init__(self, [1, 1, 1])

class RgbaColor(BasicArray):
	def __init__(self, *args):
		# FIXME: implement
		BasicArray.__init__(self, [1, 1, 1, 1])

# initialization
def MakeRgbColor(type, args):
	# FIXME: implement
	return RgbColor()
def MakeRgbaColor(type, args):
	# FIXME: implement
	return RgbaColor()
def RgbColorc(*args): return MakeRgbColor(complex, args)
def RgbColorf(*args): return MakeRgbColor(float,   args)
def RgbColori(*args): return MakeRgbColor(int,     args)
def RgbColorl(*args): return MakeRgbColor(long,    args)
def RgbaColorc(*args): return MakeRgbaColor(complex, args)
def RgbaColorf(*args): return MakeRgbaColor(float,   args)
def RgbaColori(*args): return MakeRgbaColor(int,     args)
def RgbaColorl(*args): return MakeRgbaColor(long,    args)

################################################################################
# interpolation

def Lerp(a, b, mu):
	return a + (b - a) * mu

def Bilerp(a, b, c, mu1, mu2):
	return a + (b - a) * mu1 + (c - a) * mu2;

################################################################################
# intersection

def IsPointInTriangle(p, a, b, c):
	assert len(p) == len(a) == len(b) == len(c) == 2
	return (
		(p - a).PerpDot(b - a) <= 0 and
		(p - b).PerpDot(c - b) <= 0 and
		(p - c).PerpDot(a - c) <= 0)

def GetHeightOnTriangle(p, a, b, c):
	assert len(p) == len(a) == len(b) == len(c) == 3
	mu = Barycentric(
		p.Swizzle(0, 2),
		a.Swizzle(0, 2),
		b.Swizzle(0, 2),
		c.Swizzle(0, 2))
	return Bilerp(a.y, b.y, c.y, *mu)

def GetTriangleNormal(a, b, c):
	assert len(a) == len(b) == len(c) == 3
	return (b - a).Cross(c - a).Norm()

def Barycentric(p, a, b, c):
	# Real-Time Collision Detection, Christer Ericson, 2005
	ab, ac, ap = b - a, c - a, p - a
	dbb, dbc, dcc = ab.Dot(ab), ab.Dot(ac), ac.Dot(ac)
	dpb, dpc = ap.Dot(ab), ap.Dot(ac)
	denom = dbb * dcc - dbc * dbc;
	return Vector2f(
		(dcc * dpb - dbc * dpc) / denom,
		(dbb * dpc - dbc * dpb) / denom)

