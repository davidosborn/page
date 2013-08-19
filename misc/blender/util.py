import math # sqrt
import Blender

################################################################################
# vector operations

def MulVec2(a, b):
	return Blender.Mathutils.Vector(a.x * b.x, a.y * b.y)
def MulVec3(a, b):
	return Blender.Mathutils.Vector(a.x * b.x, a.y * b.y, a.z * b.z)
def MulVec4(a, b):
	return Blender.Mathutils.Vector(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w)

def SwizzleVec2(v, i1, i2):
	return Blender.Mathutils.Vector(v[i1], v[i2])
def SwizzleVec3(v, i1, i2, i3):
	return Blender.Mathutils.Vector(v[i1], v[i2], v[i3])
def SwizzleVec4(v, i1, i2, i3, i4):
	return Blender.Mathutils.Vector(v[i1], v[i2], v[i3], v[i4])

def BlenderToNativeVec3(v):
	return Blender.Mathutils.Vector(-v.x, v.z, v.y)

################################################################################
# quaternion operations

def QuatFromVec3(v):
	return Blender.Mathutils.Quaternion(
		math.sqrt(max(1 - Blender.Mathutils.DotVecs(v, v), 0)),
		v.x, v.y, v.z)

def MulQuat(a, b):
	return Blender.Mathutils.Quaternion(
		a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z,
		a.x * b.w + a.w * b.x + a.z * b.y - a.y * b.z,
		a.y * b.w + a.w * b.y + a.x * b.z - a.z * b.x,
		a.z * b.w + a.w * b.z + a.y * b.x - a.x * b.y)

def BlenderToNativeQuat(q):
	return Blender.Mathutils.Quaternion(q.w, -q.x, q.z, q.y)

################################################################################
# matrix operations

def ScaleMatrix(v):
	return Blender.Mathutils.Matrix(
		[v.x, 0, 0, 0],
		[0, v.y, 0, 0],
		[0, 0, v.z, 0],
		[0, 0, 0, 1])

def BlenderToNativeMatrix(m):
	# NOTE: this function should only be used on a matrix comprised of a single
	# translation/rotation/scale triplet
	return (
		ScaleMatrix(SwizzleVec3(m.scalePart(), 0, 2, 1)).resize4x4() *
		BlenderToNativeQuat(m.toQuat()).toMatrix().resize4x4() *
		Blender.Mathutils.TranslationMatrix(BlenderToNativeVec3(m.translationPart())))

################################################################################
# inteprolation

def Lerp(a, b, mu):
	return a + (b - a) * mu

################################################################################
# intersection

def ClosestPointOnLineWeight(a, b, p):
	ab = b - a
	ap = p - a
	return \
		Blender.Mathutils.DotVecs(ap, ab) / \
		Blender.Mathutils.DotVecs(ab, ab)
def ClosestPointOnLine(a, b, p):
	return Lerp(a, b, ClosestPointOnLineWeight(a, b, p))
def ClosestPointOnSegment(a, b, p):
	mu = ClosestPointOnLineWeight(a, b, p)
	if mu <= 0: return a
	if mu >= 1: return b
	return Lerp(a, b, mu)

################################################################################
# bone operations

def GetBoneLocalMatrix(bone):
	matrix = bone.matrix['ARMATURESPACE'].copy()
	if bone.parent:
		matrix *= bone.parent.matrix['ARMATURESPACE'].copy().invert()
	return matrix
def GetBoneLocalPosition(bone):
	return GetBoneLocalMatrix(bone).translationPart()
def GetBoneLocalOrientation(bone):
	return bone.matrix['BONESPACE'].toQuat()
def GetBoneLocalScale(bone):
	return bone.matrix['BONESPACE'].scalePart()

# pose
def GetPoseBoneLocalMatrix(bone):
	matrix = bone.poseMatrix.copy()
	if bone.parent:
		matrix *= bone.parent.poseMatrix.copy().invert()
	return matrix
def GetPoseBoneLocalPosition(bone):
	return GetPoseBoneLocalMatrix(bone).translationPart()
def GetPoseBoneLocalOrientation(bone):
	return GetPoseBoneLocalMatrix(bone).toQuat()
def GetPoseBoneLocalScale(bone):
	return GetPoseBoneLocalMatrix(bone).scalePart()

# deforming
def IsBoneTransientForDeform(bone):
	return (bone and
		Blender.Armature.NO_DEFORM in bone.options and
		bone.name not in ('offset',))
def GetBoneDeformMatrix(bone, pose = None, native = False):
	# NOTE: returns (pose)matrix in space of closest deforming parent
	matrix = (
		GetPoseBoneLocalMatrix(pose.bones[bone.name])
		if pose else GetBoneLocalMatrix(bone))
	if native:
		matrix = BlenderToNativeMatrix(matrix)
	parent = bone.parent
	while IsBoneTransientForDeform(parent):
		parentMatrix = (
			GetPoseBoneLocalMatrix(pose.bones[parent.name])
			if pose else GetBoneLocalMatrix(parent))
		if native:
			parentMatrix = BlenderToNativeMatrix(parentMatrix)
		matrix *= parentMatrix
		parent = parent.parent
	return matrix

################################################################################
# windowing interface

def HandleEvent(event, value):
	global _waitEscKey
	if event in (Blender.Draw.MOUSEX, Blender.Draw.MOUSEY):
		return
	if event == Blender.Draw.ESCKEY:
		if _waitEscKey: Blender.Draw.Exit()
		else: _waitEscKey = True
		return
	_waitEscKey = False

################################################################################
# animation

def SetCurrentFrame(frame):
	Blender.Set('curframe', frame)
	Blender.Scene.GetCurrent().update(1)
