#!BPY

# """
# Name: 'PAGE track (.track)...'
# Blender: 243
# Group: 'Export'
# Tip: 'Export PAGE track'
# """

__version__ = 0.1
__author__ = 'David Osborn'
__email__ = 'page:david*osborn*name'
__url__ = 'http://david.osborn.name/page'
__bpydoc__ = """\
This script exports selected meshes to PAGE's native track format.
"""

import struct
import Blender
import util # SwizzleVec2

class Face:
	def __init__(self, verts, neighbours = None):
		self.verts = verts
		self.neighbours = neighbours or [None, None, None]
		self.normal = Blender.Mathutils.CrossVecs(
			self.verts[1].co - self.verts[0].co,
			self.verts[2].co - self.verts[0].co).normalize()
		self.valid = self.normal.z > .001

def edgeKey(face, i):
	i, j = face.verts[i].index, face.verts[(i + 1) % len(face.verts)].index
	return (min(i, j), max(i, j))

def Export(mesh, path):
	# tessellate quads
	faces = []
	for face in mesh.faces:
		if len(face.verts) == 3:
			faces.append(Face(face.verts))
		elif len(face.verts) == 4:
			# split face; make shortest split possible without generating
			# collinear faces
			if abs(Blender.Mathutils.DotVecs(
				(face.verts[2].co - face.verts[1].co).normalize(),
				(face.verts[3].co - face.verts[1].co).normalize())) > .9999:
				split = 0
			elif abs(Blender.Mathutils.DotVecs(
				(face.verts[1].co - face.verts[0].co).normalize(),
				(face.verts[2].co - face.verts[0].co).normalize())) > .9999:
				split = 1
			elif (
				(face.verts[0].co - face.verts[2].co).length <=
				(face.verts[1].co - face.verts[3].co).length):
				split = 0
			else:
				split = 1
			faces.extend([
				Face([face.verts[i] for i in (split, split + 1, split + 2)]),
				Face([face.verts[i] for i in (split + 2, (split + 3) % 4, split)])])
		else:
			raise Exception('cannot tessellate ngons')
	# check for collinear faces
	for face in faces:
		if abs(Blender.Mathutils.DotVecs(
			(face.verts[1].co - face.verts[0].co).normalize(),
			(face.verts[2].co - face.verts[0].co).normalize())) > .9999:
			print 'Warning: Collinear face:', faces.index(face)
			print ' ', face.verts[0].co
			print ' ', face.verts[1].co
			print ' ', face.verts[2].co
	# build edge map
	edges = {}
	for face in faces:
		for i in range(len(face.verts)):
			edges.setdefault(edgeKey(face, i), []).append(face)
	# resolve face neighbours
	for face in faces:
		for i, neighbour in enumerate(face.neighbours):
			if neighbour is None:
				neighbours = edges[edgeKey(face, i)]
				if len(neighbours) == 2:
					face.neighbours[i] = neighbours[neighbours[0] is face]
				elif len(neighbours) > 2:
					raise Exception('cannot resolve edge with more than 2 neighbouring faces')
	# remove invalid faces
	for i in range(len(faces) - 1, -1, -1):
		face = faces[i]
		if not face.valid:
			vertices = [util.SwizzleVec2(vert.co, 0, 1) for vert in face.verts]
			for j, vertex in enumerate(vertices):
				if (vertices[(j + 2) % 3] - vertex).length < .001: break
			else: continue
			neighbours = face.neighbours[j:] + face.neighbours[0:j]
			print [neighbour and neighbour.valid for neighbour in neighbours]
			if neighbours[0]:
				for k, neighbour in enumerate(neighbours[0].neighbours):
					if neighbour is face:
						neighbours[0].neighbours[k] = neighbours[1]
						break
			if neighbours[1]:
				for k, neighbour in enumerate(neighbours[1].neighbours):
					if neighbour is face:
						neighbours[1].neighbours[k] = neighbours[0]
						break
			if neighbours[2]:
				assert not neighbours[2].valid
				for k, neighbour in enumerate(neighbours[2].neighbours):
					if neighbour is face:
						neighbours[2].neighbours[k] = None
						break
			del faces[i]
	# generate face indices
	for i, face in enumerate(faces):
		face.index = i
	# write file
	file = open(path, 'wb')
	file.write('PAGEtrack')
	# write header
	file.write(struct.pack('<I', len(faces)))
	# write faces
	for face in faces:
		for vert in face.verts:
			file.write(struct.pack('<3f', -vert.co.x, vert.co.z, vert.co.y))
		for neighbour in face.neighbours:
			if neighbour is not None:
				index = neighbour.index
			else: index = -1
			file.write(struct.pack('<i', index))
	# finished
	print 'Wrote %d faces to %s' % (len(faces), path)

editMode = Blender.Window.EditMode()
Blender.Window.EditMode(0)
for obj in Blender.Object.GetSelected():
	if obj.getType() == 'Mesh':
		Blender.Window.FileSelector(lambda path: Export(Blender.Mesh.Get(obj.data.name), path), 'Export %s' % obj.name, Blender.sys.makename(ext = '.track'))
Blender.Window.EditMode(editMode)
