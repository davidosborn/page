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
