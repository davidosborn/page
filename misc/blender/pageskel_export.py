#!BPY

# """
# Name: 'PAGE skeleton (.skel)...'
# Blender: 243
# Group: 'Export'
# Tip: 'Export PAGE skeleton'
# """

__version__ = 0.1
__author__ = 'David Osborn'
__email__ = 'page:david*osborn*name'
__url__ = 'http://david.osborn.name/page'
__bpydoc__ = """\
This script exports selected armatures to PAGE's native skeleton format. Only
bones with the deform flag will be included.
"""

import struct
import traceback # print_exc
import Blender
import util # GetBoneDeformMatrix, HandleEvent

################################################################################
# preferences

def UpdatePrefs():
	prefs = Blender.Registry.GetKey('pageskel_export', True) or {}
	global applyPose
	applyPose = prefs.setdefault('apply.pose', False)

def CommitPrefs():
	prefs = {}
	prefs['apply.pose'] = applyPose
	Blender.Registry.SetKey('pageskel_export', prefs, True)

UpdatePrefs()

################################################################################
# export

def Export(object, path):
	armature = Blender.Armature.Get(object.data.name)
	pose = object.getPose()
	# ignore non-deforming bones
	bones = [bone for bone in armature.bones.values() if
		Blender.Armature.NO_DEFORM not in bone.options]
	# output skeleton file
	file = open(path, 'wb')
	file.write('PAGEskel')
	# write skeleton header
	file.write(struct.pack('<I', len(bones)))
	# write bones
	for bone in bones:
		parent = -1
		if bone.parent:
			for bone2 in bones:
				parent = parent + 1
				if bone2.name == bone.parent.name: break
			else: parent = -1
		matrix = util.GetBoneDeformMatrix(bone, pose if applyPose else None, True)
		position = matrix.translationPart()
		orientation = matrix.toQuat()
		if orientation.w < 0: orientation.negate()
		file.write(struct.pack('<I', len(bone.name)) + bone.name +
			struct.pack('<6fi',
				position.x, position.y, position.z,
				orientation.x, orientation.y, orientation.z,
				parent))
	# finished
	print 'Wrote %d bones to %s' % (len(bones), path)

def DoExport(armature, path):
	editMode = Blender.Window.EditMode()
	Blender.Window.EditMode(0)
	try: Export(armature, path)
	except: traceback.print_exc()
	Blender.Window.EditMode(editMode)

def AskExport(armature):
	Blender.Window.FileSelector(lambda path: DoExport(armature, path),
		'Export %s' % armature.getName(), Blender.sys.makename(ext = '.skel'))

################################################################################
# selection

def UpdateArmatures():
	# update armatures
	global armatures
	armatures = []
	for object in Blender.Object.GetSelected():
		if object.getType() == 'Armature':
			armatures.append(object)
	armatures.sort()
	# update selected armature
	globals().setdefault('selectedArmature', None)
	global selectedArmature
	if selectedArmature not in armatures:
		selectedArmature = armatures[0] if armatures else None

################################################################################
# user interface

EXPORT_BUTTON = 100
UPDATE_BUTTON = 101
EXIT_BUTTON   = 102
ARMATURE_MENU = 103
POSE_BUTTON   = 104

def OnDraw():
	Blender.BGL.glColor4ub(*Blender.Window.Theme.Get()[0].get('ui').text)
	Blender.BGL.glRasterPos2f(10, 10)
	Blender.Draw.Text('Export PAGE animation')
	width = Blender.Window.GetAreaSize()[0]
	global buttons
	buttons = [
		Blender.Draw.Button('Export', EXPORT_BUTTON, 10, 30, 60, 20),
		Blender.Draw.Button('Update', UPDATE_BUTTON, 80, 30, 60, 20),
		Blender.Draw.Button('Exit', EXIT_BUTTON, min(max(width - 70, 150), 250), 30, 60, 20)]
	Blender.Draw.BeginAlign()
	global armatureMenu
	armatureMenu = Blender.Draw.Menu(
		'%%tArmature|%s' % '|'.join([armature.name for armature in armatures]),
		ARMATURE_MENU, 10, 80, min(max(width - 20, 200), 300), 20,
		armatures.index(selectedArmature) + 1 if selectedArmature else 0)
	global poseButton
	poseButton = Blender.Draw.Toggle('Apply current pose', POSE_BUTTON,
		10, 60, min(max(width - 20, 200), 300), 20, applyPose)
	Blender.Draw.EndAlign()

def OnButton(event):
	global selectedArmature
	if event == EXPORT_BUTTON:
		if selectedArmature:
			AskExport(selectedArmature)
	elif event == UPDATE_BUTTON:
		UpdateArmatures()
		Blender.Draw.Redraw()
	elif event == EXIT_BUTTON:
		Blender.Draw.Exit()
		CommitPrefs()
		return
	elif event == ARMATURE_MENU:
		selectedArmature = armatures[armatureMenu.val - 1] if armatureMenu.val else None
	elif event == POSE_BUTTON:
		global applyPose
		applyPose = poseButton.val

################################################################################
# main

UpdateArmatures()
Blender.Draw.Register(OnDraw, util.HandleEvent, OnButton)
