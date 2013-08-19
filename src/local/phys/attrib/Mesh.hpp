#ifndef    page_local_phys_attrib_Mesh_hpp
#   define page_local_phys_attrib_Mesh_hpp

#	include "../../cache/fwd.hpp" // Proxy
#	include "../../util/raii/copy_ptr.hpp"

namespace page
{
	namespace res { class Mesh; }

	namespace phys
	{
		namespace attrib
		{
			struct Mesh
			{
				// construct
				Mesh();
				explicit Mesh(const cache::Proxy<res::Mesh> &);

				// access
				const cache::Proxy<res::Mesh> &GetMesh() const;
				void SetMesh(const cache::Proxy<res::Mesh> &);

				private:
				util::copy_ptr<cache::Proxy<res::Mesh>> mesh;
			};
		}
	}
}

#endif
