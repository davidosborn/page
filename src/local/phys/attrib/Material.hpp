#ifndef    page_local_phys_attrib_Material_hpp
#   define page_local_phys_attrib_Material_hpp

#	include "../../cache/proxy/Proxy.hpp"

namespace page
{
	namespace res { class Material; }

	namespace phys
	{
		namespace attrib
		{
			struct Material
			{
				// construct
				Material() = default;
				explicit Material(const cache::Proxy<res::Material> &);

				// access
				const cache::Proxy<res::Material> &GetMaterial() const;
				void SetMaterial(const cache::Proxy<res::Material> &);

				private:
				cache::Proxy<res::Material> material;
			};
		}
	}
}

#endif
