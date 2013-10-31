#ifndef    page_local_cache_proxy_AabbProxy_hpp
#   define page_local_cache_proxy_AabbProxy_hpp

#	include "../../math/fwd.hpp" // Aabb
#	include "../../util/Identifiable.hpp" // Identifiable::Id
#	include "BasicProxy.hpp"
#	include "Proxy.hpp"

namespace page { namespace phys
{
	class Bounds;
	class Form;
	namespace attrib { class Pose; }
}}

namespace page { namespace cache
{
	/**
	 * A proxy representing an axis-aligned bounding-box in the cache.
	 */
	class AabbProxy : public BasicProxy<math::Aabb<3>>
	{
		IMPLEMENT_CLONEABLE(AabbProxy, BasicProxy<math::Aabb<3>>)

		/*-------+
		| traits |
		+-------*/

		public:
		using typename BasicProxy<math::Aabb<3>>::pointer;

		/*-------------+
		| constructors |
		+-------------*/

		/**
		 * Creates an AABB proxy for an optionally-posed form.
		 */
		explicit AabbProxy(const phys::Form &, bool pose = true);

		/**
		 * Creates an AABB proxy for an optionally-posed bounding-skeleton.
		 */
		AabbProxy(const Proxy<phys::Bounds> &, const phys::attrib::Pose &);

		private:
		AabbProxy(const Proxy<phys::Bounds> &, util::Identifiable::Id);

		/*--------------------------+
		| BasicProxy implementation |
		+--------------------------*/

		pointer DoLock() const override;

		/*-------------+
		| data members |
		+-------------*/

		Proxy<phys::Bounds> bounds;
		util::Identifiable::Id poseId;
	};
}}

#endif
