namespace page { namespace phys
{
	/*-------------+
	| constructors |
	+-------------*/

	template <typename MeshInputRange>
		Bounds::Bounds(
			MeshInputRange meshes,
			const boost::optional<const res::Skeleton &> &skeleton,
			ENABLE_IF_IMPL(util::is_range<MeshInputRange>::value)) :
				Bounds(skeleton)
	{
		for (const auto &mesh : meshes)
			Add(*this, mesh);
	}
}}
