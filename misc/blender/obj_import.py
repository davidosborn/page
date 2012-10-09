#!BPY

# """
# Name: 'Wavefront (.obj)...'
# Blender: 243
# Group: 'Import'
# Tip: 'Import Wavefront object file'
# """

__version__ = 0.1
__author__ = 'David Osborn'
__email__ = 'page:david*osborn*name'
__url__ = 'http://david.osborn.name/page'
__bpydoc__ = """\
This script imports Wavefront object (.obj) files.
"""

import math, operator, os.path
import Blender

class Error(Exception):
	pass

def AngleBetween(a, b, c):
	ab = map(operator.sub, b, a)
	ac = map(operator.sub, c, a)
	lenab = math.sqrt(sum(map(operator.mul, ab, ab)))
	lenac = math.sqrt(sum(map(operator.mul, ac, ac)))
	ab = [e / lenab for e in ab] if lenab else 0
	ac = [e / lenac for e in ac] if lenac else 0
	dot = min(max(sum(map(operator.mul, ac, ab)), -1), 1)
	return math.degrees(math.acos(dot))

def CleanLines(file):
	for line in file:
		line = line.split('#')[0].strip()
		if line: yield line

class Index:
	def __init__(self, vertex = 0, texCoord = 0):
		self.vertex = vertex
		self.texCoord = texCoord

class Face:
	def __init__(self, indices = None, material = None):
		self.indices = indices or []
		self.material = material

class Object:
	def __init__(self, vertices = None, texCoords = None, faces = None):
		self.vertices = vertices or []
		self.texCoords = texCoords or []
		self.faces = faces or []

class Material:
	def __init__(self, diffuseTexture = None):
		self.diffuseTexture = diffuseTexture

def LoadMaterials(path):
	print '  Loading materials from', path
	materials = {}
	material = None
	for line in CleanLines(open(path)):
		split = line.split()
		if split[0] == 'newmtl':
			name = line[len(split[0]):].lstrip()
			if name not in materials:
				materials[name] = Material()
				material = materials[name]
			else:
				print '    Warning: duplicate material', name
				material = Material()
		elif split[0] == 'map_Kd':
			line = line[len(split[0]):].lstrip()
			split = split[1:]
			while True:
				if split[0] == '-blendu' or split[0] == '-blendv' or split[0] == '-cc' or split[0] == '-clamp':
					line = line[len(split[0])].lstrip()[len(split[1])].lstrip()
					split = split[2:]
				elif split[0] == '-mm':
					line = line[len(split[0])].lstrip()[len(split[1])].lstrip()[len(split[2])].lstrip()
					split = split[3:]
				elif split[0] == '-o' or split[0] == '-s' or split[0] == 't':
					line = line[len(split[0])].lstrip()[len(split[1])].lstrip()[len(split[2])].lstrip()[len(split[3])].lstrip()
					split = split[4:]
				else:
					material.diffuseTexture = os.path.normpath(os.path.join(os.path.dirname(path), line))
					break
	return materials

def Import(path):
	# load objects from file
	print 'Loading ' + path
	objs = {'':Object()}
	obj = objs['']
	vertexOffset, texCoordOffset = 0, 0
	materials = {}
	material = None
	for line in CleanLines(open(path)):
		split = line.split()
		if split[0] == 'o':
			vertexOffset += len(obj.vertices)
			texCoordOffset += len(obj.texCoords)
			obj = objs.setdefault(line[len(split[0]):].lstrip(), Object())
			vertexOffset -= len(obj.vertices)
			texCoordOffset -= len(obj.texCoords)
		elif split[0] == 'v':
			obj.vertices.append((-float(split[1]), float(split[3]), float(split[2])))
		elif split[0] == 'vt':
			obj.texCoords.append((float(split[1]), float(split[2])))
		elif split[0] == 'f':
			face = Face(material = material)
			for set in split[1:]:
				set = set.split('/')
				vi = int(set[0]) - (vertexOffset + 1)
				vti = int(set[1]) - (texCoordOffset + 1) if len(set) >= 2 and set[1] else 0
				face.indices.append(Index(vi, vti))
			obj.faces.append(face)
		elif split[0] == 'mtllib':
			materialPath = os.path.normpath(os.path.join(os.path.dirname(path), line[len(split[0]):].lstrip()))
			try:
				for name, mat in LoadMaterials(materialPath).iteritems():
					if name not in materials:
						materials[name] = mat
			except IOError:
				print '  Warning: Failed to load materials'
		elif split[0] == 'usemtl':
			material = line[len(split[0]):].lstrip()
			if material not in materials:
				print '  Warning: Material', name, 'not found'
				materials[material] = Material()
	if not objs[''].faces:
		del objs['']
	# verify objects
	for obj in objs.itervalues():
		if not obj.faces:
			print '  Warning: Object', obj.name, 'has no faces'
			continue
		for i, face in enumerate(obj.faces):
			if len(face.indices) < 3:
				raise Error('Face has less than 3 vertices')
			else:
				for j, index in enumerate(face.indices):
					if index in face.indices[:j]:
						print '  Warning: Face', i, 'has duplicate vertices'
						break
				else:
					for j, index in enumerate(face.indices):
						for index2 in face.indices[:j]:
							if obj.vertices[index2.vertex] == obj.vertices[index.vertex]:
								print '  Warning: Face', i, 'has overlapping vertices'
								break
						else: continue
						break
	# split fgons
	print 'Splitting fgons'
	for obj in objs.itervalues():
		fgons, obj.faces = obj.faces, []
		while fgons:
			splitFaces = []
			for face in fgons:
				if len(face.indices) > 4:
					split = (0, (0, 2))
					for i in range(1, len(face.indices) - 1):
						for j in range(i + 2, i + len(face.indices) - 1):
							minAngle = 180
							for triplet in ((i, i - 1, j), (i, j, i + 1), (j, j - 1, i), (j, i, j + 1)):
								minAngle = min(AngleBetween(*[obj.vertices[face.indices[k % len(face.indices)].vertex] for k in triplet]), minAngle)
							if minAngle > split[0]:
								split = (minAngle, (i, j))
					splitFaces.extend([
						Face([face.indices[i % len(face.indices)] for i in range(split[1][0], split[1][1] + 1)], face.material),
						Face([face.indices[i % len(face.indices)] for i in range(split[1][1], split[1][0] + len(face.indices) + 1)], face.material)])
				else: obj.faces.append(face)
			fgons = splitFaces
	# create materials
	print 'Creating materials'
	btextures = {}
	for material in materials.itervalues():
		if material.diffuseTexture:
			texture = Blender.Texture.New(os.path.splitext(os.path.basename(material.diffuseTexture))[0])
			try:
				texture.setImage(Blender.Image.Load(material.diffuseTexture))
				texture.setType('Image')
			except IOError:
				print '  Warning: failed to load image', material.diffuseTexture
			else:
				texture.setImageFlags('InterPol', 'UseAlpha', 'MipMap')
				btextures[material.diffuseTexture] = texture
	bmaterials = {}
	for name, material in materials.iteritems():
		mmat = Blender.Material.New(name)
		if material.diffuseTexture and material.diffuseTexture in btextures:
			mmat.setTexture(0, btextures[material.diffuseTexture], Blender.Texture.TexCo.UV, Blender.Texture.MapTo.COL | Blender.Texture.MapTo.ALPHA)
		bmaterials[name] = mmat
	for obj in objs.itervalues():
		hasMaterial, hasNoMaterial = False, False
		for face in obj.faces:
			if face.material is not None: hasMaterial = True
			else: hasNoMaterial = True
			if hasMaterial and hasNoMaterial:
				bmaterials[None] = Blender.Material.New('default')
				break
		else: continue
		break
	# create objects
	print 'Creating objects'
	scene = Blender.Scene.GetCurrent()
	for name, obj in objs.iteritems():
		if not name: name = os.path.splitext(os.path.basename(path))[0]
		mesh = Blender.Mesh.New(name)
		# HACK: Blender rotates faces that refer to the zero vertex at the third
		# or fourth index; we create a dummy vertex to prevent this
		mesh.verts.extend([(0, 0, 0)] + obj.vertices)
		bfaces = [[index.vertex + 1 for index in face.indices] for face in obj.faces]
		mfaceIndices = mesh.faces.extend(bfaces, ignoreDups = True, indexList = True)
		if obj.texCoords:
			for mfaceIndex, face in zip(mfaceIndices, obj.faces):
				mesh.faces[mfaceIndex].uv = [Blender.Mathutils.Vector(obj.texCoords[index.texCoord]) for index in face.indices]
		mesh.verts.delete(0)
		for face in mesh.faces:
			face.smooth = True
		mesh.calcNormals()
		mesh.mode &= ~Blender.Mesh.Modes.TWOSIDED
		# assign materials
		for face in obj.faces:
			if face.material is not None:
				matSlots = {}
				for mfaceIndex, face in zip(mfaceIndices, obj.faces):
					if face.material not in matSlots:
						matSlots[face.material] = len(mesh.materials)
						mesh.materials += [bmaterials[face.material]]
					mesh.faces[mfaceIndex].mat = matSlots[face.material]
					mtex = mesh.materials[mesh.faces[mfaceIndex].mat].getTextures()[0]
					if mtex: mesh.faces[mfaceIndex].image = mtex.tex.getImage()
				break
		# link to scene
		scene.objects.new(mesh).select(True)
	print 'Import complete'

editMode = Blender.Window.EditMode()
Blender.Window.EditMode(0)
Blender.Window.FileSelector(Import, 'Import OBJ')
Blender.Window.EditMode(editMode)
Blender.Window.Redraw()
