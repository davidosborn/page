#!BPY

# """
# Name: 'PAGE animation (.anim)...'
# Blender: 245
# Group: 'Export'
# Tip: 'Export PAGE animation'
# """

__version__ = 0.1
__author__ = 'David Osborn'
__email__ = 'page:david*osborn*name'
__url__ = 'http://david.osborn.name/page'
__bpydoc__ = """\
This script exports the selected action of the selected armature to PAGE's
native animation format. Only bones with the deform flag will be included. Only
constraints that target a bone in the armature will generate keyframes.
"""

frameRate = 24

import sys # maxint
import struct
import traceback # print_exc
import Blender
import util # GetBoneDeformMatrix, HandleEvent, SetCurrentFrame

################################################################################
# export

class Animation:
	def __init__(self, bones = None):
		self.bones = bones or []

	class Bone:
		def __init__(self, name = None, dependencies = None, frames = None):
			self.name = name
			self.dependencies = dependencies or []
			self.frames = frames or []

	class Frame:
		def __init__(self, time = 0):
			self.time = time
			self.position = Blender.Mathutils.Vector()
			self.orientation = Blender.Mathutils.Quaternion()
			self.scale = Blender.Mathutils.Vector(1, 1, 1)

def Export(object, action, path):
	assert object.getType() == 'Armature'
	armature = Blender.Armature.Get(object.data.name)
	action.setActive(object)
	pose = object.getPose()
	# create animation
	anim = Animation()
	for armBone in armature.bones.values():
		# don't export non-deforming bones
		if Blender.Armature.NO_DEFORM in armBone.options:
			continue
		# build list of dependencies
		dependencies = [armBone.name]
		for dependency in dependencies:
			# check for non-deforming parent dependency
			depArmBone = armature.bones[dependency]
			if util.IsBoneTransientForDeform(depArmBone.parent):
				if depArmBone.parent.name not in dependencies:
					dependencies.append(depArmBone.parent.name)
			# check for constraint dependencies
			# FIXME: make a distinction between location, rotation, and
			# scale constraints; for example, a bone with a rotation
			# constraint, which in turn has a location constraint, should
			# not be dependant on its constraint's constraint because the
			# bone's rotation will not be affected by the constraint's
			# change in location; this would also allow us to separate out
			# position, orientation, and scale keyframes
			depPoseBone = pose.bones[dependency]
			for constraint in depPoseBone.constraints:
				try:
					target = constraint[Blender.Constraint.Settings.TARGET]
					if target.getType() == 'Armature':
						targetArmature = Blender.Armature.Get(target.data.name)
						targetBone = targetArmature.bones[constraint[Blender.Constraint.Settings.BONE]]
						if targetBone.name not in dependencies:
							dependencies.append(targetBone.name)
				except KeyError: pass
		# determine if bone should be exported
		for dependency in dependencies:
			if dependency in action.getChannelNames():
				anim.bones.append(Animation.Bone(armBone.name, dependencies))
				break
	# generate keyframes
	originalFrame = Blender.Get('curframe')
	ipos = action.getAllChannelIpos()
	for bone in anim.bones:
		# NOTE: assuming frame 1 is the first frame
		# http://blenderartists.org/forum/showthread.php?p=966463
		currentFrame = 0
		while True:
			# find next keyframe
			nextFrame = sys.maxint
			for dependency in bone.dependencies:
				if dependency in ipos:
					for curve in ipos[dependency]:
						if curve.name in (
							'LocX', 'LocY', 'LocZ',
							'QuatX', 'QuatY', 'QuatZ', 'QuatW',
							'ScaleX', 'ScaleY', 'ScaleZ'):
							for bez in curve.bezierPoints:
								frame = int(bez.pt[0])
								if frame > currentFrame:
									nextFrame = min(frame, nextFrame)
									break
			if nextFrame == sys.maxint: break
			currentFrame = nextFrame
			util.SetCurrentFrame(currentFrame)
			# get deforming pose matrix
			matrix = util.GetBoneDeformMatrix(armature.bones[bone.name], pose, True)
			# generate frame
			frame = Animation.Frame(currentFrame)
			frame.position = matrix.translationPart()
			frame.orientation = matrix.toQuat()
			frame.scale = matrix.scalePart()
			bone.frames.append(frame)
	util.SetCurrentFrame(originalFrame)
	# count frames and calculate duration
	numFrames = 0
	duration = 0
	for bone in anim.bones:
		numFrames += len(bone.frames)
		for frame in bone.frames:
			# NOTE: assuming time starts at frame 1
			# http://blenderartists.org/forum/showthread.php?p=966463
			frame.time = float(frame.time - 1) / frameRate
			duration = max(frame.time, duration)
	# output animation file
	file = open(path, 'wb')
	file.write('PAGEanim')
	# write animation header
	file.write(struct.pack('<fI', duration, len(anim.bones)))
	# write bones
	for bone in anim.bones:
		file.write(struct.pack('<2I', len(bone.name), len(bone.frames)) + bone.name)
		# write frames
		for frame in bone.frames:
			file.write(struct.pack('<11f', frame.time, \
				frame.position.x, frame.position.y, frame.position.z, \
				frame.orientation.x, frame.orientation.y, frame.orientation.z, frame.orientation.w, \
				frame.scale.x, frame.scale.y, frame.scale.z))
	# finished
	print 'Wrote %d bones and %d frames over %.2f seconds to %s' % (len(anim.bones), numFrames, duration, path)

def DoExport(armature, action, path):
	try: Export(armature, action, path)
	except: traceback.print_exc()

def AskExport(armature, action):
	Blender.Window.FileSelector(lambda path: DoExport(armature, action, path),
		'Export %s' % action.getName(), Blender.sys.makename(ext = '.anim'))

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
	# update actions
	UpdateActions()

def UpdateActions():
	# update actions
	global actions
	actions = []
	if selectedArmature:
		for action in Blender.Armature.NLA.GetActions().itervalues():
			for channel in action.getAllChannelIpos().iterkeys():
				if channel not in selectedArmature.data.bones.keys():
					break
			else:
				actions.append(action)
		actions.sort()
	# update selected action
	globals().setdefault('selectedAction', None)
	global selectedAction
	if selectedAction not in actions:
		selectedAction = actions[0] if actions else None

################################################################################
# user interface

EXPORT_BUTTON = 100
UPDATE_BUTTON = 101
EXIT_BUTTON   = 102
ARMATURE_MENU = 103
ACTION_MENU   = 104

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
	global armatureMenu, actionMenu
	armatureMenu = Blender.Draw.Menu(
		'%%tArmature|%s' % '|'.join([armature.name for armature in armatures]),
		ARMATURE_MENU, 10, 60, min(max(width - 20, 200), 300) / 2, 20,
		armatures.index(selectedArmature) + 1 if selectedArmature else 0)
	actionMenu = Blender.Draw.Menu(
		'%%tAction|%s' % '|'.join([action.name for action in actions]),
		ACTION_MENU, 10 + min(max(width - 20, 200), 300) / 2, 60,
		min(max(width - 20, 200), 300) - min(max(width - 20, 200), 300) / 2, 20,
		actions.index(selectedAction) + 1 if selectedAction else 0)
	Blender.Draw.EndAlign()

def OnButton(event):
	global selectedArmature, selectedAction
	if event == EXPORT_BUTTON:
		if selectedArmature and selectedAction:
			AskExport(selectedArmature, selectedAction)
	elif event == UPDATE_BUTTON:
		UpdateArmatures()
		Blender.Draw.Redraw()
	elif event == EXIT_BUTTON:
		Blender.Draw.Exit()
		return
	elif event == ARMATURE_MENU:
		selectedArmature = armatures[armatureMenu.val - 1] if armatureMenu.val else None
		UpdateActions()
	elif event == ACTION_MENU:
		selectedAction = actions[actionMenu.val - 1] if actionMenu.val else None

################################################################################
# main

UpdateArmatures()
Blender.Draw.Register(OnDraw, util.HandleEvent, OnButton)
