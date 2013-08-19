#include "../../phys/aabb.hpp" // MakeAabb
#include "../../phys/attrib/Pose.hpp" // Pose->util::Identifiable, Pose::dirty{Pose,Transform}Sig
#include "../../phys/Bounds.hpp" // Bounds::bones
#include "../../phys/Form.hpp" // Form::GetModel
#include "AabbProxy.hpp"
#include "BoundsProxy.hpp"

namespace page { namespace cache
{
	/*--------+
	| deleter |
	+--------*/

	namespace
	{
		void Delete(const math::Aabb<3> *aabb, boost::signals::connection &poseCon, boost::signals::connection &transformCon)
		{
			poseCon.disconnect();
			transformCon.disconnect();
			delete aabb;
		}
	}

	/*-------------+
	| constructors |
	+-------------*/

	AabbProxy::AabbProxy(const phys::Form &form, bool pose) :
		AabbProxy(BoundsProxy((*form.GetModel()), pose), form) {}

	AabbProxy::AabbProxy(const Proxy<phys::Bounds> &bounds, const phys::attrib::Pose &pose) :
		AabbProxy(bounds, pose.GetId()) {}

	AabbProxy::AabbProxy(const Proxy<phys::Bounds> &bounds, util::Identifiable::Id poseId) :
		BasicProxy<T>(Signature("AABB", {bounds->GetSignature(), poseId})),
		bounds(bounds), poseId(poseId) {}

	/*--------------------------+
	| BasicProxy implementation |
	+--------------------------*/

	auto AabbProxy::DoLock() const -> pointer
	{
		phys::attrib::Pose &pose(util::ReferenceFromId<phys::attrib::Pose>(poseId));
		const phys::Bounds &bounds(**this->bounds);
		boost::signals::scoped_connection
			poseCon(
				pose.IsPosed() && !bounds.bones.empty() ?
				pose.dirtyPoseSig.connect(GetInvalidateFunction()) :
				boost::signals::connection()),
			transformCon(pose.dirtyTransformSig.connect(GetInvalidateFunction()));
		return Instance(new math::Aabb<3>(MakeAabb(bounds, pose)),
			std::bind(Delete, std::placeholders::_1,
				poseCon.release(), transformCon.release()));
	}
}}
