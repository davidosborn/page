#include "../../cache/proxy/ResourceProxy.hpp"
#include "Material.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			// construct
			Material::Material(const cache::Proxy<res::Material> &material) :
				material(material) {}

			// access
			const cache::Proxy<res::Material> &Material::GetMaterial() const
			{
				return *material;
			}
			void Material::SetMaterial(const cache::Proxy<res::Material> &material)
			{
				this->material = material;
			}
		}
	}
}
