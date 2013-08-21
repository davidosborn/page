#ifndef    page_local_cache_proxy_SkinProxy_hpp
#   define page_local_cache_proxy_SkinProxy_hpp

#	include "../../util/Identifiable.hpp" // Identifiable::Id
#	include "BasicProxy.hpp"
#	include "Proxy.hpp"

namespace page
{
	namespace phys
	{
		class Skin;
		namespace attrib { class Pose; }
	}
	namespace res { class Mesh; }
}

namespace page { namespace cache
{
	/**
	 * A proxy representing an instance of phys::Skin in the cache.
	 */
	class SkinProxy :
		public BasicProxy<phys::Skin>,
		public virtual util::Cloneable<SkinProxy, BasicProxy<phys::Skin>>
	{
		/*-------+
		| traits |
		+-------*/

		public:
		using typename BasicProxy<phys::Skin>::pointer;

		/*-------------+
		| constructors |
		+-------------*/

		SkinProxy(const Proxy<res::Mesh> &, const phys::attrib::Pose &);

		private:
		SkinProxy(const Proxy<res::Mesh> &, util::Identifiable::Id &poseId);

		/*--------------------------+
		| BasicProxy implementation |
		+--------------------------*/

		pointer DoLock() const override;

		/*-------------+
		| data members |
		+-------------*/

		Proxy<res::Mesh> mesh;
		util::Identifiable::Id poseId;
	};
}}

#endif
