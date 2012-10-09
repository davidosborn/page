#!BPY

# """
# Name: 'Twoside off'
# Blender: 240
# Group: 'Mesh'
# Tip: 'Switch to single-sided mode'
# """

__version__ = 0.1
__author__ = 'David Osborn'
__email__ = 'page:david*osborn*name'
__url__ = 'http://david.osborn.name/page'
__bpydoc__ = """\
This script switches selected faces to single-sided mode.
"""

import Blender

editMode = Blender.Window.EditMode()
Blender.Window.EditMode(0)
for obj in Blender.Object.GetSelected():
	if obj.getType() == 'Mesh':
		mesh = Blender.Mesh.Get(obj.data.name)
		for face in mesh.faces:
			if face.sel: face.mode &= ~Blender.Mesh.FaceModes.TWOSIDE
Blender.Window.EditMode(editMode)
Blender.Window.Redraw()
