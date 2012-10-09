#!BPY

# """
# Name: '3D Studio (.3ds)...'
# Blender: 245
# Group: 'Import'
# Tip: 'Import 3D Studio file'
# """

__version__ = 0.1
__author__ = 'David Osborn'
__email__ = 'page:david*osborn*name'
__url__ = 'http://david.osborn.name/page'
__bpydoc__ = """\
This script imports 3D Studio (.3ds) files.
"""

import struct
import Blender

MAIN3DS          = 0x4d4d
EDIT3DS          = 0x3d3d
EDIT_OBJECT      = 0x4000
OBJ_TRIMESH      = 0x4100
TRI_VERTEXL      = 0x4110
TRI_FACEL1       = 0x4120
TRI_MAPPINGCOORS = 0x4140

class FormatError(Exception):
	def __init__(self, value = 'Format error'):
		self.value = value
	def __str__(self):
		return repr(self.value)

class Chunk:
	def __init__(self, data = (0, 0)):
		self.id = data[0]
		self.size = data[1]

def Import(path):
	file = open(path, 'rb')
	# check the root chunk
	chunkStack = [Chunk(struct.unpack('<HI', file.read(6)))]
	if chunkStack[-1].id != MAIN3DS: raise FormatError
	chunkStack[-1].size -= 6
	tell = file.tell()
	meshes = []
	while 1:
		chunkStack[-1].size -= file.tell() - tell
		while not chunkStack[-1].size:
			del chunkStack[-1]
			if not len(chunkStack): break
		if not len(chunkStack): break
		if chunkStack[-1].size < 0: raise FormatError
		chunkStack.append(Chunk(struct.unpack('<HI', file.read(6))))
		chunkStack[-2].size -= chunkStack[-1].size
		chunkStack[-1].size -= 6
		tell = file.tell()
		if chunkStack[-2].id == MAIN3DS:
			if chunkStack[-1].id == EDIT3DS:
				continue
		elif chunkStack[-2].id == EDIT3DS:
			if chunkStack[-1].id == EDIT_OBJECT:
				name = ''
				while 1:
					ch = file.read(1)
					if ch == '\x00': break
					name += ch
				continue
		elif chunkStack[-2].id == EDIT_OBJECT:
			if chunkStack[-1].id == OBJ_TRIMESH:
				meshes.append(Blender.NMesh.New(name))
				continue
		elif chunkStack[-2].id == OBJ_TRIMESH:
			if chunkStack[-1].id == TRI_VERTEXL:
				for i in range(struct.unpack('<H', file.read(2))[0]):
					x, y, z = struct.unpack('<3f', file.read(12))
					meshes[-1].verts.append(Blender.NMesh.Vert(-x, -y, z))
				continue
			elif chunkStack[-1].id == TRI_FACEL1:
				for i in range(struct.unpack('<H', file.read(2))[0]):
					meshes[-1].addFace(Blender.NMesh.Face([meshes[-1].verts[i] for i in struct.unpack('<HHHxx', file.read(8))]))
				continue
			elif chunkStack[-1].id == TRI_MAPPINGCOORS:
				for i in range(struct.unpack('<H', file.read(2))[0]):
					u, v = struct.unpack('<2f', file.read(8))
					meshes[-1].verts[i].uvco = u, v
				continue
		# skip over this chunk
		file.seek(tell + chunkStack[-1].size)
	# done reading; now finish importing
	for mesh in meshes:
		# copy texture coordinates to faces
		mesh.hasFaceUV(True)
		for face in mesh.faces:
			face.uv = [(vert.uvco.x, vert.uvco.y) for vert in face.v]
		# remove duplicate vertices
		for vi, vert in enumerate(mesh.verts):
			for vert2 in mesh.verts[vi + 1:]:
				if vert.co == vert2.co:
					for face in mesh.faces:
						for i in range(len(face.v)):
							if face.v[i] is vert2:
								face.v[i] = vert
					mesh.verts.remove(vert2)
		# calculate vertex indices
		for i in range(len(mesh.verts)):
			mesh.verts[i].index = i
		# set default attributes
		for face in mesh.faces:
			face.smooth = True
		mesh.update(True)
		mesh.mode &= ~Blender.NMesh.Modes.TWOSIDED
		# instantiate
		scene = Blender.Scene.GetCurrent()
		scene.objects.new(mesh).select(True)
	# finished
	numFaces = 0
	numVerts = 0
	for mesh in meshes:
		numFaces += len(mesh.faces)
		numVerts += len(mesh.verts)
	print 'Imported %d meshes with %d faces and %d vertices' % (len(meshes), numFaces, numVerts)

editMode = Blender.Window.EditMode()
Blender.Window.EditMode(0)
Blender.Window.FileSelector(Import, 'Import 3ds')
Blender.Window.EditMode(editMode)
Blender.Window.Redraw()
