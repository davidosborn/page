#include <functional> // mem_fn
#include <sstream> // ostringstream

#include <boost/range/adaptor/indirected.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/algorithm/copy.hpp>

#include "../../util/io/separated_ostream_iterator.hpp"

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
				BasicProxy<phys::Bounds>(MakeSignature(meshes, skeleton)),
				meshes(meshes), skeleton(skeleton)
	{
		Init();
	}

	template <typename MeshInputRange>
		std::string BoundsProxy::MakeSignature(
			MeshInputRange meshes,
			const Proxy<res::Skeleton> &skeleton,
			ENABLE_IF(util::is_range<MeshInputRange>::value))
	{
		std::ostringstream ss;
		boost::copy(
			boost::adaptors::transform(
				boost::adaptors::indirect(meshes),
				std::mem_fn(&Proxy<res::Mesh>::GetSignature)),
			util::separated_ostream_iterator<std::string>(ss, ','));

		if (skeleton)
			// NOTE: using alternate separator to differentiate from mesh
			ss << '+' << skeleton->GetSource();

		return Signature("bounds", ss.str());
	}
}}
