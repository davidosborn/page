#ifndef    page_local_cache_proxy_Bounds_hpp
#   define page_local_cache_proxy_Bounds_hpp

#	include <vector>

#	include "../../util/type_traits/range.hpp" // is_range
#	include "../../util/type_traits/sfinae.hpp" // ENABLE_IF
#	include "BasicProxy.hpp"
#	include "Proxy.hpp"

namespace page
{
	namespace phys { class Bounds; }
	namespace res
	{
		class Mesh;
		class Model;
		class Skeleton;
	}
}

namespace page { namespace cache
{
	/**
	 * A proxy representing a bounding skeleton in the cache.
	 */
	class BoundsProxy :
		public BasicProxy<phys::Bounds>,
		public virtual util::Cloneable<BoundsProxy, BasicProxy<phys::Bounds>>
	{
		/*-------+
		| traits |
		+-------*/

		public:
		using typename BasicProxy<phys::Bounds>::pointer;

		/*-------------+
		| constructors |
		+-------------*/

		/**
		 * Creates a bounding-skeleton proxy for an optionally-posed model.
		 */
		explicit BoundsProxy(const res::Model &, bool pose = true);

		/**
		 * Creates a bounding-skeleton proxy for a range of meshes and an
		 * optional skeleton.
		 *
		 * @param[in] meshes A range of mesh proxies.
		 * @param[in] skeleton An optional skeleton proxy.
		 */
		template <typename MeshInputRange>
			explicit BoundsProxy(
				MeshInputRange meshes,
				const Proxy<res::Skeleton> &skeleton = nullptr,
				ENABLE_IF(util::is_range<MeshInputRange>::value));

		private:
		/**
		 * Performs some additional initialization for the constructors.
		 */
		void Init();

		/*--------------------------+
		| BasicProxy implementation |
		+--------------------------*/

		pointer DoLock() const override;

		/*-------------+
		| data members |
		+-------------*/

		std::vector<Proxy<res::Mesh>> meshes;
		Proxy<res::Skeleton> skeleton;
	};
}}

#	include "BoundsProxy.tpp"
#endif
