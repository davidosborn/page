#!BPY

# """
# Name: 'PAGE mesh (.mesh)...'
# Blender: 243
# Group: 'Export'
# Tip: 'Export PAGE mesh'
# """

__version__ = 0.1
__author__ = 'David Osborn'
__email__ = 'page:david*osborn*name'
__url__ = 'http://david.osborn.name/page'
__bpydoc__ = """\
This script exports selected meshes to PAGE's native mesh format.
"""

import os, struct
import traceback # print_exc
import Blender
import util # BlenderToNativeVec3, HandleEvent, ScaleMatrix

################################################################################
# preferences

def UpdatePrefs():
	prefs = Blender.Registry.GetKey('pagemesh_export', True) or {}
	global applyModifiers, applyTransform, exportDir
	applyModifiers = prefs.setdefault('apply.modifiers', False)
	applyTransform = prefs.setdefault('apply.transform', False)
	exportDir = prefs.setdefault('export.dir', '')

def CommitPrefs():
	prefs = {}
	prefs['apply.modifiers'] = applyModifiers
	prefs['apply.transform'] = applyTransform
	prefs['export.dir'] = exportDir
	Blender.Registry.SetKey('pagemesh_export', prefs, True)

UpdatePrefs()

################################################################################
# export

class Mesh:
	class Face:
		def __init__(self, vertices = None, twoSided = False):
			self.vertices = vertices or []
			self.twoSided = twoSided

	class Vertex:
		def __init__(self, co = None, no = None, uv = None, influences = None, index = 0):
			self.co = co or Blender.Mathutils.Vector(0, 0, 0)
			self.no = no or Blender.Mathutils.Vector(0, 0, 1)
			self.uv = uv or Blender.Mathutils.Vector(0, 0)
			self.influences = influences or []
			self.index = index

	class Influence:
		def __init__(self, name = '', weight = 1):
			self.name = name
			self.weight = weight

	def __init__(self, faces = None, vertices = None):
		self.faces = faces or []
		self.vertices = vertices or []

	def FromBlenderMesh(self, obj, matMask = 0xffff):
		self.__init__()
		# get mesh from object
		mesh = Blender.Mesh.New()
		mesh.getFromObject(obj, not applyModifiers)
		if applyTransform:
			mesh.transform(obj.matrix, True)
		obj.__copy__().link(mesh)
		# copy faces and vertices
		faceMap, vertMap = {}, {}
		for face in mesh.faces:
			if 1 << face.mat & matMask:
				for vert in face.verts:
					if vert.index not in vertMap:
						vertex = Mesh.Vertex(vert.co, vert.no, index = len(self.vertices))
						vertMap[vert.index] = vertex
						self.vertices.append(vertex)
				faceMap[len(self.faces)] = face
				self.faces.append(Mesh.Face([vertMap[vert.index] for vert in face.verts], mesh.mode & Blender.NMesh.Modes.TWOSIDED or mesh.faceUV and face.mode & Blender.NMesh.FaceModes.TWOSIDE))
		# copy influences
		for group in mesh.getVertGroupNames():
			for index, weight in mesh.getVertsFromGroup(group, 1):
				if index in vertMap:
					vertMap[index].influences.append(Mesh.Influence(group, weight))
		# copy texture coordinates
		if mesh.faceUV:
			# split vertices at texture seams
			print '  Splitting texture seams'
			vertexUvFaceIndices = {}
			for i, face in enumerate(self.faces):
				for j, vertex in enumerate(face.vertices):
					uv = faceMap[i].uv[j]
					vertexUvFaceIndices.setdefault(vertex.index, {}).setdefault((uv.x, uv.y), []).append((face, j))
			for vertex, uvFaceIndices in vertexUvFaceIndices.iteritems():
				vertex = self.vertices[vertex]
				iter = uvFaceIndices.iteritems()
				vertex.uv = Blender.Mathutils.Vector(*iter.next()[0])
				for uv, faceIndices in iter:
					newVertex = Mesh.Vertex(vertex.co, vertex.no, Blender.Mathutils.Vector(*uv), vertex.influences, len(self.vertices))
					self.vertices.append(newVertex)
					for face, index in faceIndices:
						face.vertices[index] = newVertex
		elif mesh.vertexUV:
			for vertex, vert in zip(self.vertices, mesh.verts):
				vertex.uv = vert.uvco
		return self

	def Clean(self):
		# remove bad faces
		self.faces = [face for face in self.faces if \
			len(face.vertices) >= 3 and \
			face.vertices[0].co != face.vertices[1].co and \
			face.vertices[1].co != face.vertices[2].co and \
			face.vertices[2].co != face.vertices[0].co]
		# remove orphaned vertices
		vertexRefCounts = [0 for i in range(len(self.vertices))]
		for face in self.faces:
			for vertex in face.vertices:
				vertexRefCounts[vertex.index] += 1
		self.vertices = [vertex for vertex in self.vertices if vertexRefCounts[vertex.index]]
		for i, vertex in enumerate(self.vertices):
			vertex.index = i

	def Tessellate(self):
		# convert quads to triangles
		# FIXME: use greatest-minimum-angle fgon splitting from obj_import
		print '  Tessellating quads'
		for face in self.faces[:]:
			if len(face.vertices) > 3:
				for i in range(2, len(face.vertices) - 1):
					self.faces.append(Mesh.Face([face.vertices[0], face.vertices[i], face.vertices[i + 1]], face.twoSided))
				del face.vertices[3:]

	def BakeTwoSided(self):
		# duplicate double-sided faces
		print '  Baking double-sided faces'
		for face in self.faces:
			if face.twoSided:
				vertices = face.vertices[:]
				vertices.reverse()
				self.faces.append(Mesh.Face(vertices))

	def Sort(self):
		# sort faces and vertices for cache coherence
		# assumes all faces are triangles
		# Linear-Speed Vertex Cache Optimization, Tom Forsyth, 2006
		# http://www.eelpi.gotdns.org/papers/fast_vert_cache_opt.html
		print '  Sorting for cache coherence'

		cacheSize = 32
		cacheDecay = 1.5
		lastFaceScore = 0.75
		valenceBoostScale = 2.0
		valenceBoostPower = 0.5

		class Face:
			def __init__(self, base = None, vertices = None):
				self.base = base
				self.vertices = vertices or []
				self.cached = False
				self.score = -3

			def UpdateScore(self):
				self.score = sum([vertex.score for vertex in face.vertices])

		class Vertex:
			def __init__(self, base = None, faces = None):
				self.base = base
				self.faces = faces or []
				self.valence = len(self.faces)
				self.cacheIndex = None
				self.score = -1

			def UpdateScore(self):
				if not self.valence: return -1.0
				if self.cacheIndex is None: self.score = 0.0
				elif self.cacheIndex < 3: self.score = lastFaceScore
				else: self.score = pow(1.0 - (self.cacheIndex - 3) * 1.0 / (cacheSize - 3), cacheDecay)
				self.score += valenceBoostScale * pow(self.valence, -valenceBoostPower)

		def TestCache(cacheSize):
			cache, misses = [], 0
			for face in self.faces:
				for vertex in face.vertices:
					if vertex.index not in cache:
						cache.insert(0, vertex.index)
						cache = cache[:cacheSize]
						misses += 1
			return 1.0 - float(misses) / (len(self.faces) * 3)

		# print initial efficiency
		print '    Starting %.2f%% efficiency' % (TestCache(cacheSize) * 100)
		# initialize faces and vertices
		vertices = [Vertex(vertex) for vertex in self.vertices]
		faces = [Face(face, [vertices[vertex.index] for vertex in face.vertices]) for face in self.faces]
		for i, face in enumerate(self.faces):
			assert(len(face.vertices) == 3)
			for vertex in face.vertices:
				vertices[vertex.index].faces.append(faces[i])
		for vertex in vertices:
			vertex.valence = len(vertex.faces)
		# calculate initial scores
		for vertex in vertices: vertex.UpdateScore()
		for face in faces: face.UpdateScore()
		# find initial best scoring face
		bestFace = Face()
		for face in faces:
			if face.score > bestFace.score:
				bestFace = face
		# build draw list
		drawn, cache = [], []
		while len(drawn) < len(faces):
			# add face to draw list
			drawn.append(bestFace)
			# move vertices to head of cache
			for vertex in bestFace.vertices:
				vertex.valence -= 1
				bestFaceIndex = vertex.faces.index(bestFace)
				vertex.faces[bestFaceIndex], vertex.faces[vertex.valence] = \
					vertex.faces[vertex.valence], vertex.faces[bestFaceIndex]
				if vertex.cacheIndex is not None: del cache[vertex.cacheIndex]
				cache.insert(0, vertex)
			# update scores and find next best scoring face
			bestFace = Face()
			for i, vertex in enumerate(cache):
				vertex.cacheIndex = i < cacheSize and i or None
				vertex.UpdateScore()
			for vertex in cache:
				for face in vertex.faces[:vertex.valence]:
					face.UpdateScore()
					if face.score > bestFace.score:
						bestFace = face
			# if best face not found, check all remaining faces
			if bestFace.score == -3:
				for vertex in vertices: vertex.UpdateScore()
				for vertex in vertices:
					for face in vertex.faces[:vertex.valence]:
						face.UpdateScore()
						if face.score > bestFace.score:
							bestFace = face
			cache = cache[:cacheSize]
		# reorder faces and vertices to match draw list
		self.faces, self.vertices = [], []
		vertexMap = [None for i in range(len(vertices))]
		for face in drawn:
			for vertex in face.vertices:
				if vertexMap[vertex.base.index] is None:
					vertexMap[vertex.base.index] = len(self.vertices)
					self.vertices.append(vertex.base)
			self.faces.append(Mesh.Face([self.vertices[vertexMap[vertex.base.index]] for vertex in face.vertices], face.base.twoSided))
		for i, vertex in enumerate(self.vertices):
			vertex.index = i
		# print results
		print '    Achieved %.2f%% efficiency' % (TestCache(cacheSize) * 100)

def Export(path, obj, matMask = 0xffff):
	print 'Exporting', os.path.splitext(os.path.basename(path))[0]
	mesh = Mesh().FromBlenderMesh(obj, matMask)
	mesh.Clean()
	mesh.Tessellate()
	mesh.BakeTwoSided()
	mesh.Sort()
	# prepare influences
	numInfluences = 0
	influenceNameIndices = {}
	for vertex in mesh.vertices:
		numInfluences += len(vertex.influences)
		for influence in vertex.influences:
			if influence.name not in influenceNameIndices:
				influenceNameIndices[influence.name] = 0
	for i, key in enumerate(influenceNameIndices):
		influenceNameIndices[key] = i
	# create mesh file
	file = open(path, 'wb')
	file.write('PAGEmesh')
	# write header
	file.write(struct.pack('<4I', len(mesh.faces), len(mesh.vertices), numInfluences, len(influenceNameIndices)))
	# write faces
	for face in mesh.faces:
		file.write(struct.pack('<3I', face.vertices[0].index, face.vertices[1].index, face.vertices[2].index))
	# write vertices
	influenceIndex = 0
	for vertex in mesh.vertices:
		co = util.BlenderToNativeVec3(vertex.co)
		no = util.BlenderToNativeVec3(vertex.no)
		file.write(struct.pack('<8f2I', co.x, co.y, co.z, no.x, no.y, no.z, vertex.uv.x, -vertex.uv.y, influenceIndex, len(vertex.influences)))
		influenceIndex += len(vertex.influences)
	# write influences
	for vertex in mesh.vertices:
		for influence in vertex.influences:
			file.write(struct.pack('<If', influenceNameIndices[influence.name], influence.weight))
	# write influence names
	for name in influenceNameIndices.iterkeys():
		file.write(struct.pack('<I', len(name)) + name)
	# finished
	print '  Wrote', len(mesh.faces), 'faces and', len(mesh.vertices), 'vertices to', path

def DoExport(path, obj, matMask = 0xffff):
	editMode = Blender.Window.EditMode()
	Blender.Window.EditMode(0)
	try:
		Export(path, obj, matMask)
	except:
		traceback.print_exc()
		return False
	finally:
		Blender.Window.EditMode(editMode)
	return True

################################################################################
# selection

class Selected:
	def __init__(self, name, obj, matMask = 0xffff):
		self.name = name
		self.group = obj.name
		self.obj = obj
		self.matMask = matMask

def UpdateSelection():
	global selected
	selected = []
	for obj in Blender.Object.GetSelected():
		if obj.getType() == 'Mesh':
			mesh = Blender.Mesh.Get(obj.data.name)
			materials = [obj.colbits & 1 << i and objMat or meshMat \
				for i, (objMat, meshMat) in enumerate(map(None, obj.getMaterials(), mesh.materials))]
			if materials:
				matRefCounts = [0 for i in range(len(materials))]
				for face in mesh.faces:
					matRefCounts[face.mat] += 1
				matNameMasks = {}
				for i, refCount in enumerate(matRefCounts):
					if refCount:
						name = materials[i] and materials[i].name or obj.name
						if name not in matNameMasks:
							matNameMasks[name] = 0
						matNameMasks[name] |= 1 << i
				if len(matNameMasks) == 1:
					selected.append(Selected(obj.name, obj, matNameMasks.values()[0]))
				else:
					for name, matMask in matNameMasks.iteritems():
						selected.append(Selected(name, obj, matMask))
			else:
				selected.append(Selected(obj.name, obj))

def ExportSelection():
	if not selected: return
	targets = {}
	for sel in selected:
		if sel.name in targets:
			raise Exception('More than one target named', sel.name)
		targets[sel.name] = sel
	targets = zip(*sorted(targets.items()))[1]
	if not os.path.exists(exportDir):
		if AskCreateDir(exportDir): os.makedirs(exportDir)
		else: return
	for target in targets:
		if not DoExport(os.path.join(exportDir, target.name + '.mesh'), target.obj, target.matMask):
			return
	print 'Finished'

################################################################################
# user interface

EXPORT_BUTTON    = 100
UPDATE_BUTTON    = 101
EXIT_BUTTON      = 102
DIR_STRING       = 103
MOD_BUTTON       = 104
TRANSFORM_BUTTON = 105

def OnDraw():
	Blender.BGL.glColor4ub(*Blender.Window.Theme.Get()[0].get('ui').text)
	Blender.BGL.glRasterPos2f(10, 10)
	Blender.Draw.Text('Export PAGE mesh')
	width = Blender.Window.GetAreaSize()[0]
	global buttons
	buttons = [
		Blender.Draw.Button('Export', EXPORT_BUTTON, 10, 30, 60, 20),
		Blender.Draw.Button('Update', UPDATE_BUTTON, 80, 30, 60, 20),
		Blender.Draw.Button('Exit', EXIT_BUTTON, min(max(width - 70, 150), 250), 30, 60, 20)]
	Blender.Draw.BeginAlign()
	global dirString
	dirString = Blender.Draw.String('Directory: ', DIR_STRING, 10, 100, min(max(width - 20, 200), 300), 20, exportDir, 256)
	global modButton
	modButton = Blender.Draw.Toggle('Apply modifiers', MOD_BUTTON, 10, 80, min(max(width - 20, 200), 300), 20, applyModifiers)
	global transformButton
	transformButton = Blender.Draw.Toggle('Apply object transform', TRANSFORM_BUTTON, 10, 60, min(max(width - 20, 200), 300), 20, applyTransform)
	Blender.Draw.EndAlign()
	# display current selection
	groupParts = {}
	for sel in selected:
		groupParts.setdefault(sel.group, []).append(sel.name)
	groupParts = groupParts.items()
	groupParts.sort()
	groupParts.reverse()
	i = 0
	for group, parts in groupParts:
		parts.sort()
		parts.reverse()
		for part in parts:
			Blender.BGL.glRasterPos2f(10, i * 20 + 130)
			Blender.Draw.Text('\t' + part)
			i += 1
		Blender.BGL.glRasterPos2f(10, i * 20 + 130)
		Blender.Draw.Text(group)
		i += 1

def OnButton(event):
	if event == EXPORT_BUTTON:
		UpdateSelection()
		ExportSelection()
	elif event == UPDATE_BUTTON:
		UpdateSelection()
		Blender.Draw.Redraw()
	elif event == EXIT_BUTTON:
		Blender.Draw.Exit()
		CommitPrefs()
		return
	elif event == DIR_STRING:
		global exportDir
		exportDir = os.path.normpath(dirString.val)
	elif event == MOD_BUTTON:
		global applyModifiers
		applyModifiers = modButton.val
	elif event == TRANSFORM_BUTTON:
		global applyTransform
		applyTransform = transformButton.val

def AskCreateDir(dir):
	return Blender.Draw.PupMenu('Directory "' + dir + '" does not exist%t|Create|Abort') == 1

################################################################################
# main

UpdateSelection()
Blender.Draw.Register(OnDraw, util.HandleEvent, OnButton)
