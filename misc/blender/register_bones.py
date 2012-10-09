#!BPY

# """
# Name: 'Register bones (pose)'
# Blender: 245
# Group: 'Misc'
# Tip: 'Translate multiple bones from reference bone to cursor'
# """

__version__ = 0.1
__author__ = 'David Osborn'
__email__ = 'page:david*osborn*name'
__url__ = 'http://david.osborn.name/page'
__bpydoc__ = """\
This script translates multiple bones from a reference bone to the cursor in
pose mode.
"""

import Blender
import util # HandleEvent

################################################################################
# execution

def Move():
	armatures = []
	for obj in Blender.Object.GetSelected():
		if obj.getType() == 'Armature':
			armature = Blender.Armature.Get(obj.data.name)
			armatures.append(armature.name)
			pose = obj.getPose()
			for bone in pose.bones.values():
				if bone.sel:
					bone.loc += (
						armature.bones[bone.name].matrix['ARMATURESPACE'] *
						obj.matrixWorld) * translation
			pose.update()
	# update meshes with modified pose
	for obj in Blender.Scene.GetCurrent().objects:
		for modifier in obj.modifiers:
			if modifier.type == Blender.Modifier.Types.ARMATURE:
				if modifier[Blender.Modifier.Settings.OBJECT].name in armatures:
					obj.data.update()

################################################################################
# translation

def UpdateTranslation():
	global translation
	translation = Blender.Mathutils.Vector(0, 0, 0)
	bonePositions = []
	for obj in Blender.Object.GetSelected():
		if obj.getType() == 'Armature':
			pose = obj.getPose()
			for bone in pose.bones.values():
				if bone.sel:
					bonePositions.append((bone.poseMatrix * obj.matrixWorld).translationPart())
	if bonePositions:
		bonePosition = Blender.Mathutils.Vector(0, 0, 0)
		for position in bonePositions: bonePosition += position
		bonePosition /= len(bonePositions)
		cursorPosition = Blender.Mathutils.Vector(Blender.Window.GetCursorPos())
		translation = cursorPosition - bonePosition

################################################################################
# user interface

MOVE_BUTTON      = 100
REFERENCE_BUTTON = 101
EXIT_BUTTON      = 102

def OnDraw():
	Blender.BGL.glColor4ub(*Blender.Window.Theme.Get()[0].get('ui').text)
	Blender.BGL.glRasterPos2f(10, 10)
	Blender.Draw.Text('Register bones')
	width = Blender.Window.GetAreaSize()[0]
	global buttons
	buttons = [
		Blender.Draw.Button('Move', MOVE_BUTTON, 10, 30, 60, 20),
		Blender.Draw.Button('Reference', REFERENCE_BUTTON, 80, 30, 80, 20),
		Blender.Draw.Button('Exit', EXIT_BUTTON, min(max(width - 70, 170), 250), 30, 60, 20)]
	# display current translation
	Blender.BGL.glRasterPos2f(10, 60)
	Blender.Draw.Text('Translation: %.2f, %.2f, %.2f' % tuple(translation))

def OnButton(event):
	if event == MOVE_BUTTON:
		Move()
		Blender.Window.Redraw()
	elif event == REFERENCE_BUTTON:
		UpdateTranslation()
		Blender.Draw.Redraw()
	elif event == EXIT_BUTTON:
		Blender.Draw.Exit()
		return

################################################################################
# main

UpdateTranslation()
Blender.Draw.Register(OnDraw, util.HandleEvent, OnButton)
