#ifndef    page_local_phys_attrib_Material_hpp
#   define page_local_phys_attrib_Material_hpp

#	include "../../cache/proxy/Proxy.hpp"

namespace page { namespace res { class Material; }}

namespace page { namespace phys { namespace attrib
{
	class Material
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		Material() = default;
		Material(const cache::Proxy<res::Material> &);

		/*----------+
		| accessors |
		+----------*/

		const cache::Proxy<res::Material> &GetMaterial() const;
		void SetMaterial(const cache::Proxy<res::Material> &);

		private:
		cache::Proxy<res::Material> value;
	};
}}}

#endif
