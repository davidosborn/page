#include "../../cache/proxy/ResourceProxy.hpp"
#include "Material.hpp"

namespace page { namespace phys { namespace attrib
{
	/*-------------+
	| constructors |
	+-------------*/

	Material::Material(const cache::Proxy<res::Material> &value) :
		value(value) {}

	/*----------+
	| accessors |
	+----------*/

	const cache::Proxy<res::Material> &Material::GetMaterial() const
	{
		return value;
	}

	void Material::SetMaterial(const cache::Proxy<res::Material> &value)
	{
		this->value = value;
	}
}}}
