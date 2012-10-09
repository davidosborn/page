#!BPY

# """
# Name: 'Check Weights'
# Blender: 245
# Group: 'Mesh'
# Tip: 'Check for vertex weights that don't add up to 1'
# """

__version__ = 0.1
__author__ = 'David Osborn'
__email__ = 'page:david*osborn*name'
__url__ = 'http://david.osborn.name/page'
__bpydoc__ = """\
This script checks for vertex weights that don't add up to 1.
"""

import Blender

editMode = Blender.Window.EditMode()
Blender.Window.EditMode(0)
for obj in Blender.Object.GetSelected():
	if obj.getType() == 'Mesh':
		mesh = obj.data
		# deselect all faces
		for face in mesh.faces:
			face.sel = 0
		# switch to vertex selection mode
		Blender.Mesh.Mode(Blender.Mesh.SelectModes.VERTEX)
		# calculate total weight for each vertex
		vertexWeights = [0 for vert in mesh.verts]
		for group in mesh.getVertGroupNames():
			for index, weight in mesh.getVertsFromGroup(group, 1):
				vertexWeights[index] += weight
		# select vertices whose weight does not add up
		badWeights = 0
		for vert, vertexWeight in zip(mesh.verts, vertexWeights):
			vert.sel = vertexWeight != 1
			if vert.sel: badWeight += 1
		mesh.update()
		print 'Found %u vertices with bad weights' % badWeights
Blender.Window.EditMode(editMode)
Blender.Window.Redraw()
