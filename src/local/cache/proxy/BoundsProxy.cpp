#include <cassert>

#include <boost/range/algorithm/find.hpp>
#include <boost/range/algorithm/sort.hpp>
#include <boost/range/algorithm/unique.hpp>
#include <boost/range/algorithm_ext/erase.hpp>

#include "../../phys/Bounds.hpp"
#include "../../res/type/Model.hpp"
#include "../../util/iterator/indirect_iterator.hpp"
#include "BoundsProxy.hpp"

namespace page { namespace cache
{
	/*-------------+
	| constructors |
	+-------------*/

	BoundsProxy::BoundsProxy(const res::Model &model, bool pose) :
		BoundsProxy(
			GetGeometricallyDistinctMeshes(model),
			pose ? model.skeleton : Proxy<res::Skeleton()) {}

	void BoundProxy::Init()
	{
		// ensure dependencies are valid
		assert(boost::find(meshes, nullptr) == meshes.end());

		// uniquely sort meshes by signature
		boost::sort(meshes);
		boost::erase(meshes, boost::unique<boost::return_found_end>(meshes));
	}

	/*--------------------------+
	| BasicProxy implementation |
	+--------------------------*/

	auto BoundsProxy::DoLock() const -> pointer
	{
		return std::make_shared<phys::Bounds>(
			util::make_indirect_range(util::make_indirect_range(meshes)),
			*skeleton);
	}
}}
