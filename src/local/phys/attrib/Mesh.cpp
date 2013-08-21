#include "../../cache/proxy/ResourceProxy.hpp"
#include "Mesh.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			// construct
			Mesh::Mesh(const cache::Proxy<res::Mesh> &mesh) :
				mesh(mesh) {}

			// access
			const cache::Proxy<res::Mesh> &Mesh::GetMesh() const
			{
				return *mesh;
			}
			void Mesh::SetMesh(const cache::Proxy<res::Mesh> &mesh)
			{
				this->mesh = mesh;
			}
		}
	}
}
