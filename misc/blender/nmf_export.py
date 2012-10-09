#!BPY

# """
# Name: 'ATI normal mapping file (.nmf)...'
# Blender: 240
# Group: 'Export'
# Tip: 'Export ATI normal mapping file'
# """

__version__ = 0.1
__author__ = 'David Osborn'
__email__ = 'page:david*osborn*name'
__url__ = 'http://david.osborn.name/page'
__bpydoc__ = """\
This script exports selected meshes to ATI's normal mapping format (.nmf).
"""

import struct
import Blender

def Export(mesh, path):
	# FIXME: tesselate quads and fgons
	# output the file
	file = open(path, 'wb')
	file.write(struct.pack('<4sI', 'NMF ', 16 + len(mesh.faces) * 96))
	file.write(struct.pack('<4s2I', 'TRIS', 8 + len(mesh.faces) * 96, len(mesh.faces)))
	for face in mesh.faces:
		for vert in face.v:
			file.write(struct.pack('<3f', vert.co.x, vert.co.y, vert.co.z))
		for vert in face.v:
			file.write(struct.pack('<3f', vert.no.x, vert.no.y, vert.no.z))
		if mesh.hasFaceUV():
			for texc in face.uv:
				file.write(struct.pack('<2f', texc[0], texc[1]))
		else:
			for vert in face.v:
				file.write(struct.pack('<2f', vert.uvco.x, vert.uvco.y))
	# finished
	print 'Wrote %d triangles to %s' % (len(mesh.faces), path)

editMode = Blender.Window.EditMode()
Blender.Window.EditMode(0)
for obj in Blender.Object.GetSelected():
	if obj.getType() == 'Mesh':
		Blender.Window.FileSelector(lambda path: Export(obj.data, path), 'Export %s' % obj.name)
Blender.Window.EditMode(editMode)
