#include <cassert>
#include <functional> // mem_fn
#include <sstream> // ostringstream

#include <boost/iterator/indirect_iterator.hpp>
#include <boost/range/adaptor/indirected.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/range/algorithm/find.hpp>
#include <boost/range/algorithm/sort.hpp>
#include <boost/range/algorithm/unique.hpp>
#include <boost/range/algorithm_ext/erase.hpp>

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
		// ensure pointers are valid
		assert(boost::find(meshes, nullptr) == meshes.end());

		// source-comparison function
		auto compareSource = [](Proxy<res::Mesh> &a, Proxy<res::Mesh> &b)
			{ return a->GetSource() < b->GetSource(); };

		// uniquely sort meshes by source
		boost::sort(meshes, compareSource);
		boost::erase(meshes, boost::unique<boost::return_found_end>(meshes, compareSource));
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
				std::mem_fn(&Proxy<res::Mesh>::GetSource)),
			util::separated_ostream_iterator<std::string>(ss, ','));

		if (skeleton)
			// NOTE: using alternate separator to differentiate from mesh
			ss << '+' << skeleton->GetSource();

		return ss.str();
	}
}}
