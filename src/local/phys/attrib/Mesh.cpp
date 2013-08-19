#include "../../cache/proxy/Resource.hpp"
#include "Mesh.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			// construct
			Mesh::Mesh() : mesh(cache::Resource<res::Mesh>().Copy()) {}
			Mesh::Mesh(const cache::Proxy<res::Mesh> &mesh) :
				mesh(mesh.Copy()) {}

			// access
			const cache::Proxy<res::Mesh> &Mesh::GetMesh() const
			{
				return *mesh;
			}
			void Mesh::SetMesh(const cache::Proxy<res::Mesh> &mesh)
			{
				this->mesh = mesh.Copy();
			}
		}
	}
}
