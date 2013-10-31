#include "../../cache/proxy/ResourceProxy.hpp"
#include "Mesh.hpp"

namespace page { namespace phys { namespace attrib
{
	/*-------------+
	| constructors |
	+-------------*/

	Mesh::Mesh(const cache::Proxy<res::Mesh> &value) :
		value(value) {}

	/*----------+
	| accessors |
	+----------*/

	const cache::Proxy<res::Mesh> &Mesh::GetMesh() const
	{
		return *value;
	}

	void Mesh::SetMesh(const cache::Proxy<res::Mesh> &value)
	{
		this->value = value;
	}
}}}
