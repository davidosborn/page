#!BPY

# """
# Name: 'Reparent bones'
# Blender: 245
# Group: 'Misc'
# Tip: 'Switches bone parents while maintaining IPOs in reference space'
# """

__version__ = 0.1
__author__ = 'David Osborn'
__email__ = 'page:david*osborn*name'
__url__ = 'http://david.osborn.name/page'
__bpydoc__ = """\
This script switches bone parents while maintaining IPOs in reference space.
"""

import Blender
import util # HandleEvent

################################################################################
# state

class State:
	class Button: pass

	class Selection:
		def __init__(self):
			self.armature = None
			self.bones    = []
			for obj in Blender.Object.GetSelected():
				if obj.getType() == 'Armature':
					self.armature = Blender.Armature.Get(obj.data.name)
					for bone in self.armature.bones.values():
						if Blender.Armature.BONE_SELECTED in bone.options:
							self.bones.append(bone)
					break

	def __init__(self):
		self.button    = self.Button()
		self.selection = self.Selection()
		self.parent    = None
		self.space     = 0
		self.parents   = []
		self._UpdateParents()

	def Update(self):
		self.selection = self.Selection()
		self._UpdateParents()

	def _UpdateParents(self):
		self.parents = (
			sorted(self.selection.armature.bones.values(),
				key = lambda bone: bone.name)
			if self.selection.armature else [])
		if self.parent not in self.parents:
			self.parent = None

state = State()

################################################################################
# execution

def Reparent():
	for bone in state.selection.bones:
		if bone is state.parent:
			print 'Warning: Cannot parent %s to itself' % bone.name
			continue
		for action in Blender.Armature.NLA.GetActions().itervalues():
			ipo = action.getChannelIpo(bone.name):
			if ipo:
				for curve in ipo:
					if curve.name in (
						'LocX', 'LocY', 'LocZ',
						'QuatX', 'QuatY', 'QuatZ', 'QuatW',
						'ScaleX', 'ScaleY', 'ScaleZ'):
						for bez in curve.bezierPoints:
							pass # FIXME: implement

################################################################################
# user interface

state.button.REPARENT = 100
state.button.UPDATE   = 101
state.button.EXIT     = 102
state.button.PARENT   = 103
state.button.SPACE    = 104

def OnDraw():
	Blender.BGL.glColor4ub(*Blender.Window.Theme.Get()[0].get('ui').text)
	Blender.BGL.glRasterPos2f(10, 10)
	Blender.Draw.Text('Reparent bones')
	width = Blender.Window.GetAreaSize()[0]
	# operations
	state.button.reparent = Blender.Draw.Button('Reparent', state.button.REPARENT, 10, 30, 80, 20)
	state.button.update   = Blender.Draw.Button('Update', state.button.UPDATE, 100, 30, 80, 20)
	state.button.exit     = Blender.Draw.Button('Exit', state.button.EXIT, min(max(width - 70, 190), 250), 30, 60, 20)
	# options
	Blender.Draw.BeginAlign()
	state.button.parent = Blender.Draw.Menu(
		'Parent%t| |' + '|'.join([bone.name for bone in state.parents]),
		state.button.PARENT,
		10, 80, min(max(width - 20, 150), 300), 20,
		state.parents.index(state.parent) + 2 if state.parent else 1)
	state.button.space = Blender.Draw.Menu(
		'Space%t|Object|World',
		state.button.SPACE,
		10, 60, min(max(width - 20, 150), 300), 20,
		state.space + 1)
	Blender.Draw.EndAlign()
	# current selection
	for i, bone in enumerate(reversed(sorted(state.selection.bones))):
		assert isinstance(bone, Blender.Armature.Bone)
		Blender.BGL.glRasterPos2f(10, i * 20 + 110)
		Blender.Draw.Text(bone.name)

def OnButton(event):
	if event == state.button.REPARENT:
		Reparent()
		Blender.Window.Redraw()
	elif event == state.button.UPDATE:
		state.Update()
		Blender.Draw.Redraw()
	elif event == state.button.EXIT:
		Blender.Draw.Exit()
		return
	elif event == state.button.PARENT:
		state.parent = (
			state.parents[state.button.parent.val - 2]
			if state.button.parent.val > 1 else None)
	elif event == state.button.SPACE:
		state.space = state.button.space.val - 1

################################################################################
# main

Blender.Draw.Register(OnDraw, util.HandleEvent, OnButton)
