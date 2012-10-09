#!BPY

# """
# Name: 'Scale to height'
# Blender: 243
# Group: 'Mesh'
# Tip: 'Scale mesh to height in Imperial units'
# """

__version__ = 0.1
__author__ = 'David Osborn'
__email__ = 'page:david*osborn*name'
__url__ = 'http://david.osborn.name/page'
__bpydoc__ = """\
This script scales mesh objects to a given height in Imperial units.
"""

import Blender
import util # HandleEvent

maxFootHeight = 11

def VerticalRange(mesh):
	lowest = mesh.verts[0].co.z
	highest = mesh.verts[0].co.z
	for vert in mesh.verts:
		lowest = min(vert.co.z, lowest)
		highest = max(vert.co.z, highest)
	return lowest, highest

def MetricToImperial(meters):
	feet = min(int(meters / 0.3048), maxFootHeight)
	inches = min(int((meters - feet * 0.3048) / 0.0254), 11)
	return feet, inches

def ImperialToMetric(feet, inches):
	return feet * 0.3048 + inches * 0.0254

################################################################################
# execution

def Scale(sel):
	editMode = Blender.Window.EditMode()
	Blender.Window.EditMode(0)
	# calculate scale
	lowest, highest = VerticalRange(sel.mesh)
	currentHeight = highest - lowest
	targetHeight = ImperialToMetric(sel.footHeight, sel.inchHeight)
	scale = targetHeight / currentHeight
	center = Blender.Mathutils.Vector(0, 0, lowest)
	matrix = Blender.Mathutils.Matrix(
		Blender.Mathutils.TranslationMatrix(-center).resize4x4() *
		Blender.Mathutils.ScaleMatrix(scale, 3).resize4x4() *
		Blender.Mathutils.TranslationMatrix(center).resize4x4())
	# scale mesh
	sel.mesh.transform(matrix)
	sel.mesh.update()
	# scale armature
	if sel.armature:
		sel.armature.makeEditable()
		for bone in sel.armature.bones.values():
			bone.head = matrix * bone.head
			bone.tail = matrix * bone.tail
		sel.armature.update()
	Blender.Window.EditMode(editMode)
	Blender.Window.Redraw()
	print 'Scaled %s to %d\'%d"' % (sel.name, sel.footHeight, sel.inchHeight)

################################################################################
# selection

class Selected:
	def __init__(self, obj):
		self.name = obj.name
		self.mesh = Blender.Mesh.Get(obj.data.name)
		# check for armature modifier
		for mod in obj.modifiers:
			if mod.type == Blender.Modifier.Types.ARMATURE:
				self.armature = mod[Blender.Modifier.Settings.OBJECT].data
				break
		else: self.armature = None
		# calculate initial height
		lowest, highest = VerticalRange(self.mesh)
		self.footHeight, self.inchHeight = MetricToImperial(highest - lowest + 0.00001)

def UpdateSelection():
	global selected
	selected = []
	for obj in Blender.Object.GetSelected():
		if obj.getType() == 'Mesh':
			selected.append(Selected(obj))

################################################################################
# user interface

APPLY_BUTTON  = 100
UPDATE_BUTTON = 101
EXIT_BUTTON   = 102
HEIGHT_SLIDER = 200

def OnDraw():
	Blender.BGL.glColor4ub(*Blender.Window.Theme.Get()[0].get('ui').text)
	Blender.BGL.glRasterPos2f(10, 10)
	Blender.Draw.Text('Scale to height')
	width = Blender.Window.GetAreaSize()[0]
	global buttons
	buttons = [
		Blender.Draw.Button('Apply', APPLY_BUTTON, 10, 30, 60, 20),
		Blender.Draw.Button('Update', UPDATE_BUTTON, 80, 30, 60, 20),
		Blender.Draw.Button('Exit', EXIT_BUTTON, min(max(width - 70, 150), 250), 30, 60, 20)]
	global sliders
	sliders = []
	global selected
	for i, sel in enumerate(selected):
		Blender.BGL.glRasterPos2f(10, i * 80 + 120)
		Blender.Draw.Text('Properties of ' + sel.name)
		sliders.extend([
			Blender.Draw.Slider('Height in feet: ', HEIGHT_SLIDER + i * 2, 10, i * 80 + 90, min(max(width - 20, 200), 300), 20, sel.footHeight, 0, maxFootHeight, False),
			Blender.Draw.Slider('Height in inches: ', HEIGHT_SLIDER + i * 2 + 1, 10, i * 80 + 60, min(max(width - 20, 200), 300), 20, sel.inchHeight, 0, 11, False)])

def OnButton(event):
	global selected
	if event == APPLY_BUTTON:
		for sel in selected:
			Scale(sel)
	elif event == UPDATE_BUTTON:
		UpdateSelection()
		Blender.Draw.Redraw()
	elif event == EXIT_BUTTON:
		Blender.Draw.Exit()
		return
	elif event >= HEIGHT_SLIDER:
		if not (event - HEIGHT_SLIDER) % 2:
			selected[(event - HEIGHT_SLIDER) / 2].footHeight = sliders[event - HEIGHT_SLIDER].val
		else:
			selected[(event - HEIGHT_SLIDER) / 2].inchHeight = sliders[event - HEIGHT_SLIDER].val

################################################################################
# main

UpdateSelection()
Blender.Draw.Register(OnDraw, util.HandleEvent, OnButton)
