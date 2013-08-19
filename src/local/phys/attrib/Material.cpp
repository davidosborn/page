#include "../../cache/proxy/Resource.hpp"
#include "Material.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib
		{
			// construct
			Material::Material() :
				material(cache::Resource<res::Material>().Copy()) {}
			Material::Material(const cache::Proxy<res::Material> &material) :
				material(material.Copy()) {}

			// access
			const cache::Proxy<res::Material> &Material::GetMaterial() const
			{
				return *material;
			}
			void Material::SetMaterial(const cache::Proxy<res::Material> &material)
			{
				this->material = material.Copy();
			}
		}
	}
}
