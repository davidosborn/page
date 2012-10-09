#!BPY

# """
# Name: 'Separate by material'
# Blender: 243
# Group: 'Mesh'
# Tip: 'Separate mesh by material''
# """

__version__ = 0.1
__author__ = 'David Osborn'
__email__ = 'page:david*osborn*name'
__url__ = 'http://david.osborn.name/page'
__bpydoc__ = """\
This script separates mesh objects by material.
"""

import copy
import Blender

class Part:
	def __init__(self, material, verts = None, faces = None):
		self.material = material
		self.verts = verts or []
		self.faces = faces or []

def SeparateMaterial(obj):
	mesh = Blender.Mesh.Get(obj.data.name)
	materials = [obj.colbits & 1 << i and objMaterial or meshMaterial for i, (objMaterial, meshMaterial) in enumerate(map(None, obj.getMaterials(), mesh.materials))]
	if not materials:
		print 'no materials to separate'
		return
	parts = [Part(material) for material in materials]
	for face in mesh.faces:
		for vert in face.verts:
			if vert.index not in parts[face.mat].verts:
				parts[face.mat].verts.append(vert.index)
		parts[face.mat].faces.append(face.index)
	for i, part in enumerate(parts[:]):
		if not part.faces: del parts[i]
	if len(parts) < 2: return
	# reuse input mesh if name matches
	reuseMesh = False
	for i, part in enumerate(parts):
		if part.material.name == mesh.name:
			parts[i], parts[-1] = parts[-1], parts[i]
			reuseMesh = True
			break
	scene = Blender.Scene.GetCurrent()
	for part in parts:
		print part.material.name
		if part is parts[-1] and reuseMesh: partMesh = mesh
		else: partMesh = copy.copy(mesh)
		partMesh.faces.delete(True, [face.index for face in partMesh.faces if face.index not in part.faces])
		partMesh.verts.delete([vert.index for vert in partMesh.verts if vert.index not in part.verts])
		partMesh.materials = [part.material]
		for face in partMesh.faces: face.mat = 0
		if part is not parts[-1] or not reuseMesh:
			partObj = copy.copy(obj)
			partObj.setName(part.material.name)
			partObj.link(partMesh)
			partObj.select(True)
			scene.link(partObj)
			# HACK: NMesh required to set name of mesh
			partNMesh = Blender.NMesh.GetRaw(partMesh.name)
			partNMesh.name = part.material.name
			partNMesh.update()
	if not reuseMesh:
		scene.unlink(obj)
		# FIXME: this function appears to be missing from Blender 2.4.3
#		Blender.Mesh.Unlink(mesh.name)

editMode = Blender.Window.EditMode()
Blender.Window.EditMode(0)
for obj in Blender.Object.GetSelected():
	if obj.getType() == 'Mesh': SeparateMaterial(obj)
Blender.Window.EditMode(editMode)
