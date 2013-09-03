#include <iterator> // begin, end

namespace page { namespace cache
{
	/*-------------+
	| constructors |
	+-------------*/

	template <typename MeshInputRange>
		BoundsProxy::BoundsProxy(
			MeshInputRange meshes,
			const Proxy<res::Skeleton> &skeleton,
			ENABLE_IF_IMPL(util::is_range<MeshInputRange>::value)) :
				BasicProxy<phys::Bounds>(Signature("bounds", meshes, skeleton)),
				meshes(std::begin(meshes), std::end(meshes)), skeleton(skeleton)
	{
		Init();
	}
}}
