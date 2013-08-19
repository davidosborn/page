#include "Mesh.hpp"
#include "Registry.hpp" // REGISTER_TYPE

namespace page
{
	namespace res
	{
		// construct/assign
		Mesh::Mesh() {}
		Mesh::Mesh(const Mesh &other) :
			bones(other.bones), vertices(other.vertices), faces(other.faces)
		{
			// remap bone iterators
			// FIXME: implement
			// remap vertex iterators
			// FIXME: implement
		}
		Mesh &Mesh::operator =(const Mesh &other)
		{
			bones = other.bones;
			vertices = other.vertices;
			faces = other.faces;
			// remap bone iterators
			// FIXME: implement
			// remap vertex iterators
			// FIXME: implement
		}

		void GenNormals(Mesh &mesh)
		{
			// FIXME: implement; do not generate normals if already existing
		}
		void SortInfluences(Mesh &mesh)
		{
			// FIXME: implement; do not sort influences if already sorted
		}

		void PostLoadMesh(Mesh &mesh)
		{
			GenNormals(mesh);
			SortInfluences(mesh);
		}

		REGISTER_TYPE(Mesh, "mesh", PostLoadMesh)
	}
}
