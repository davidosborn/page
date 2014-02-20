/**
 * @copyright
 *
 * Copyright (c) 2006-2014 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the same place
 *    and form as other copyright, license, and disclaimer information.
 *
 * 3. Redistributions in binary form must also include an acknowledgement in the
 *    same place and form as other acknowledgements (such as the credits),
 *    similar in substance to the following:
 *
 *       Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#include "../../phys/aabb.hpp" // MakeAabb
#include "../../phys/attrib/Pose.hpp" // Pose->util::Identifiable, Pose::dirty{Pose,Transform}Sig
#include "../../phys/Bounds.hpp" // Bounds::bones
#include "../../phys/node/Form.hpp" // Form::GetModel
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
		BasicProxy<math::Aabb<3>>(Signature("AABB", bounds, poseId)),
		bounds(bounds), poseId(poseId) {}

	/*--------------------------+
	| BasicProxy implementation |
	+--------------------------*/

	auto AabbProxy::DoLock() const -> pointer
	{
		phys::attrib::Pose &pose(util::GetReferenceById<phys::attrib::Pose>(poseId));
		const phys::Bounds &bounds(*this->bounds);
		boost::signals::scoped_connection
			poseCon(
				pose.IsPosed() && !bounds.bones.empty() ?
				pose.dirtyPoseSig.connect(GetInvalidateFunction()) :
				boost::signals::connection()),
			transformCon(pose.dirtyTransformSig.connect(GetInvalidateFunction()));
		return pointer(new math::Aabb<3>(MakeAabb(bounds, pose)),
			std::bind(Delete, std::placeholders::_1,
				poseCon.release(), transformCon.release()));
	}
}}
