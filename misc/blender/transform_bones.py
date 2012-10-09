#!BPY

# """
# Name: 'Transform bones (pose)'
# Blender: 245
# Group: 'Misc'
# Tip: 'Apply numeric transformation to bones'
# """

__version__ = 0.1
__author__ = 'David Osborn'
__email__ = 'page:david*osborn*name'
__url__ = 'http://david.osborn.name/page'
__bpydoc__ = """\
This script applies a numeric transformation to multiple bones in pose mode.
"""

import Blender
import util # HandleEvent, Mul{Quat,Vec3}

################################################################################
# initial state

loc   = Blender.Mathutils.Vector(0, 0, 0)
rot   = Blender.Mathutils.Euler(0, 0, 0)
scale = Blender.Mathutils.Vector(1, 1, 1)

################################################################################
# preferences

def UpdatePrefs():
	prefs = Blender.Registry.GetKey('transform_bones', True) or {}
	global globalSpace
	globalSpace = prefs.setdefault('global', False)

def CommitPrefs():
	prefs = {}
	prefs['global'] = globalSpace
	Blender.Registry.SetKey('transform_bones', prefs, True)

UpdatePrefs()

################################################################################
# execution

def Transform():
	armatures = []
	for obj in Blender.Object.GetSelected():
		if obj.getType() == 'Armature':
			armature = Blender.Armature.Get(obj.data.name)
			armatures.append(armature.name)
			pose = obj.getPose()
			for bone in pose.bones.values():
				if bone.sel:
					if globalSpace:
						matrix = (
							armature.bones[bone.name].matrix['ARMATURESPACE'] *
							obj.matrixWorld)
						bone.loc += matrix * loc
						# FIXME: implement rotation and scaling
					else:
						bone.loc += loc
						bone.quat = util.MulQuat(rot.toQuat(), bone.quat)
						bone.size = util.MulVec3(bone.size, scale)
			pose.update()
	# update meshes with modified pose
	for obj in Blender.Scene.GetCurrent().objects:
		for modifier in obj.modifiers:
			if modifier.type == Blender.Modifier.Types.ARMATURE:
				if modifier[Blender.Modifier.Settings.OBJECT].name in armatures:
					obj.data.update()

################################################################################
# user interface

TRANSFORM_BUTTON = 100
EXIT_BUTTON      = 101
GLOBAL_BUTTON    = 102
LOCX_STRING      = 103
LOCY_STRING      = 104
LOCZ_STRING      = 105
ROTX_STRING      = 106
ROTY_STRING      = 107
ROTZ_STRING      = 108
SCALEX_STRING    = 109
SCALEY_STRING    = 110
SCALEZ_STRING    = 111

def OnDraw():
	Blender.BGL.glColor4ub(*Blender.Window.Theme.Get()[0].get('ui').text)
	Blender.BGL.glRasterPos2f(10, 10)
	Blender.Draw.Text('Transform bones')
	width = Blender.Window.GetAreaSize()[0]
	# create buttons
	global buttons
	buttons = [
		Blender.Draw.Button('Transform', TRANSFORM_BUTTON, 10, 30, 80, 20),
		Blender.Draw.Button('Exit', EXIT_BUTTON, min(max(width - 70, 100), 250), 30, 60, 20)]
	# create global button
	global globalButton
	globalButton = Blender.Draw.Toggle('Global', GLOBAL_BUTTON, 10, 60, min(max(width - 20, 150), 300), 20, globalSpace)
	# create transformation buttons
	global transformStrings
	transformStrings = []
	Blender.Draw.BeginAlign()
	transformStrings.extend([
		Blender.Draw.String('LocX: ', LOCX_STRING, 10, 270, min(max(width - 20, 150), 300), 20, str(loc.x), 10, '', OnTransformButton),
		Blender.Draw.String('LocY: ', LOCY_STRING, 10, 250, min(max(width - 20, 150), 300), 20, str(loc.y), 10, '', OnTransformButton),
		Blender.Draw.String('LocZ: ', LOCZ_STRING, 10, 230, min(max(width - 20, 150), 300), 20, str(loc.z), 10, '', OnTransformButton)])
	Blender.Draw.EndAlign()
	Blender.Draw.BeginAlign()
	transformStrings.extend([
		Blender.Draw.String('RotX: ', ROTX_STRING, 10, 200, min(max(width - 20, 150), 300), 20, str(rot.x), 10, '', OnTransformButton),
		Blender.Draw.String('RotY: ', ROTY_STRING, 10, 180, min(max(width - 20, 150), 300), 20, str(rot.y), 10, '', OnTransformButton),
		Blender.Draw.String('RotZ: ', ROTZ_STRING, 10, 160, min(max(width - 20, 150), 300), 20, str(rot.z), 10, '', OnTransformButton)])
	Blender.Draw.EndAlign()
	Blender.Draw.BeginAlign()
	transformStrings.extend([
		Blender.Draw.String('ScaleX: ', SCALEX_STRING, 10, 130, min(max(width - 20, 150), 300), 20, str(scale.x), 10, '', OnTransformButton),
		Blender.Draw.String('ScaleY: ', SCALEY_STRING, 10, 110, min(max(width - 20, 150), 300), 20, str(scale.y), 10, '', OnTransformButton),
		Blender.Draw.String('ScaleZ: ', SCALEZ_STRING, 10, 90,  min(max(width - 20, 150), 300), 20, str(scale.z), 10, '', OnTransformButton)])
	Blender.Draw.EndAlign()

def OnButton(event):
	if event == TRANSFORM_BUTTON:
		Transform()
		Blender.Window.Redraw()
	elif event == EXIT_BUTTON:
		Blender.Draw.Exit()
		CommitPrefs()
		return
	elif event == GLOBAL_BUTTON:
		global globalSpace
		globalSpace = bool(globalButton.val)

def OnTransformButton(event, value):
	global loc, rot, scale
	if   event == LOCX_STRING:   loc.x   = float(value)
	elif event == LOCY_STRING:   loc.y   = float(value)
	elif event == LOCZ_STRING:   loc.z   = float(value)
	elif event == ROTX_STRING:   rot.x   = float(value)
	elif event == ROTY_STRING:   rot.y   = float(value)
	elif event == ROTZ_STRING:   rot.z   = float(value)
	elif event == SCALEX_STRING: scale.x = float(value)
	elif event == SCALEY_STRING: scale.y = float(value)
	elif event == SCALEZ_STRING: scale.z = float(value)

################################################################################
# main

Blender.Draw.Register(OnDraw, util.HandleEvent, OnButton)
