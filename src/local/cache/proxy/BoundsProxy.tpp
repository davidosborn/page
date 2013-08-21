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
				BasicProxy(Signature("bounds", meshes, skeleton)),
				meshes(meshes), skeleton(skeleton)
	{
		Init();
	}
}}
