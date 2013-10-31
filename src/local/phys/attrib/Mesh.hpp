#ifndef    page_local_phys_attrib_Mesh_hpp
#   define page_local_phys_attrib_Mesh_hpp

#	include "../../cache/proxy/Proxy.hpp"

namespace page { namespace res { class Mesh; }}

namespace page { namespace phys { namespace attrib
{
	class Mesh
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		Mesh() = default;
		Mesh(const cache::Proxy<res::Mesh> &);

		/*----------+
		| accessors |
		+----------*/

		const cache::Proxy<res::Mesh> &GetMesh() const;
		void SetMesh(const cache::Proxy<res::Mesh> &);

		private:
		cache::Proxy<res::Mesh> value;
	};
}}}

#endif
