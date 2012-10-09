#!BPY

# """
# Name: 'Bone forward'
# Blender: 245
# Group: 'Misc'
# Tip: 'Set roll to point bones forward'
# """

__version__ = 0.1
__author__ = 'David Osborn'
__email__ = 'page:david*osborn*name'
__url__ = 'http://david.osborn.name/page'
__bpydoc__ = """\
This script modifies the roll of selected bones to align their up vector. The
target up vector is calculated from the closest point on the bone to the cursor,
perpendicular to the bone. If the cursor is disabled, the world-space Z axis, or
in the case of vertical bones, the Y axis, will be used instead.
"""

import Blender
import util # ClosestPointOnLine, HandleEvent

################################################################################
# preferences

def UpdatePrefs():
	prefs = Blender.Registry.GetKey('bone_forward', True) or {}
	global useCursor
	useCursor = prefs.setdefault('cursor.enabled', False)

def CommitPrefs():
	prefs = {}
	prefs['cursor.enabled'] = useCursor
	Blender.Registry.SetKey('bone_forward', prefs, True)

UpdatePrefs()

################################################################################
# execution

def ExecuteSelection():
	editMode = Blender.Window.EditMode()
	Blender.Window.EditMode(0)
	global selected
	for obj, bones in selected.iteritems():
		armature = Blender.Armature.Get(obj.data.name)
		armature.makeEditable()
		for name in bones:
			bone = armature.bones[name]
			if Blender.Armature.BONE_SELECTED in bone.options:
				matrix = obj.getInverseMatrix().rotationPart() * bone.matrix.rotationPart().invert()
				boneForward = matrix * Blender.Mathutils.Vector(0, 1, 0)
				source = matrix * Blender.Mathutils.Vector(0, 0, 1)
				if useCursor:
					cursorPos = Blender.Mathutils.Vector(Blender.Window.GetCursorPos())
					target = cursorPos - util.ClosestPointOnLine(bone.head, bone.tail, cursorPos)
					if \
						abs(target.x) < .001 and \
						abs(target.y) < .001 and \
						abs(target.z) < .001:
						raise Exception('Cursor is collinear to %s' % bone.name)
					target.normalize()
				elif \
					abs(bone.tail.x - bone.head.x) < .001 and \
					abs(bone.tail.y - bone.head.y) < .001:
					target = Blender.Mathutils.Vector(0, 1, 0)
					if bone.tail.z > bone.head.z:
						target.y = -target.y
				else:
					target = Blender.Mathutils.CrossVecs(
						Blender.Mathutils.CrossVecs(boneForward, Blender.Mathutils.Vector(0, 0, 1)),
						boneForward)
				delta = Blender.Mathutils.AngleBetweenVecs(source, target)
				if Blender.Mathutils.DotVecs(
					Blender.Mathutils.CrossVecs(source, target),
					boneForward) < 0:
					delta = -delta
				bone.roll += delta
				print 'Adjusted roll of %s by %u degrees' % (bone.name, delta)
		armature.update()
	Blender.Window.EditMode(editMode)
	Blender.Window.Redraw()

################################################################################
# selection

def UpdateSelection():
	editMode = Blender.Window.EditMode()
	Blender.Window.EditMode(0)
	global selected
	selected = {}
	for obj in Blender.Object.GetSelected():
		if obj.getType() == 'Armature':
			selected[obj] = []
			armature = Blender.Armature.Get(obj.data.name)
			for bone in armature.bones.values():
				if Blender.Armature.BONE_SELECTED in bone.options:
					selected[obj].append(bone.name)
			if not selected[obj]:
				del selected[obj]
	Blender.Window.EditMode(editMode)

################################################################################
# user interface

EXECUTE_BUTTON = 100
UPDATE_BUTTON  = 101
EXIT_BUTTON    = 102
CURSOR_BUTTON  = 103

def OnDraw():
	Blender.BGL.glColor4ub(*Blender.Window.Theme.Get()[0].get('ui').text)
	Blender.BGL.glRasterPos2f(10, 10)
	Blender.Draw.Text('Point bones forward')
	width = Blender.Window.GetAreaSize()[0]
	global buttons
	buttons = [
		Blender.Draw.Button('Execute', EXECUTE_BUTTON, 10, 30, 60, 20),
		Blender.Draw.Button('Update', UPDATE_BUTTON, 80, 30, 60, 20),
		Blender.Draw.Button('Exit', EXIT_BUTTON, min(max(width - 70, 150), 250), 30, 60, 20)]
	global cursorButton
	cursorButton = Blender.Draw.Toggle('Use Cursor', CURSOR_BUTTON, 10, 60, min(max(width - 20, 200), 300), 20, useCursor)
	# display current selection
	global selected
	armatureBones = {}
	for obj, bones in selected.iteritems():
		armatureBones[obj.data.name] = bones
	armatureBones = armatureBones.items()
	armatureBones.sort()
	armatureBones.reverse()
	i = 0
	for armature, bones in armatureBones:
		bones.sort()
		bones.reverse()
		for bone in bones:
			Blender.BGL.glRasterPos2f(10, i * 20 + 90)
			Blender.Draw.Text('\t' + bone)
			i += 1
		Blender.BGL.glRasterPos2f(10, i * 20 + 90)
		Blender.Draw.Text(armature)
		i += 1

def OnButton(event):
	if event == EXECUTE_BUTTON:
		UpdateSelection()
		ExecuteSelection()
		Blender.Draw.Exit()
		CommitPrefs()
		return
	elif event == UPDATE_BUTTON:
		UpdateSelection()
		Blender.Draw.Redraw()
	elif event == EXIT_BUTTON:
		Blender.Draw.Exit()
		CommitPrefs()
		return
	elif event == CURSOR_BUTTON:
		global useCursor
		useCursor = bool(cursorButton.val)

################################################################################
# main

UpdateSelection()
Blender.Draw.Register(OnDraw, util.HandleEvent, OnButton)
