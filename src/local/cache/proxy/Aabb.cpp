/**
 * @section license
 *
 * Copyright (c) 2006-2012 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.

 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution, and in the same
 *    place and form as other copyright, license, and disclaimer information.
 *
 * As a special exception, distributions of derivative works in binary form may
 * include an acknowledgement in place of the above copyright notice, this list
 * of conditions, and the following disclaimer in the documentation and/or other
 * materials provided with the distribution, and in the same place and form as
 * other acknowledgements, similar in substance to the following:
 *
 *    Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#include <cassert>
#include "../../phys/aabb.hpp" // MakeAabb
#include "../../phys/attrib/Pose.hpp" // Pose->util::Identifiable, Pose::dirty{Pose,Transform}Sig
#include "../../phys/Bounds.hpp" // Bounds::bones
#include "../../phys/Form.hpp" // Form::GetModel
#include "../../util/lexical_cast.hpp"
#include "Aabb.hpp"
#include "Bounds.hpp"

namespace page
{
	namespace cache
	{
		namespace
		{
			// deleter
			void Delete(const math::Aabb<3> *aabb, util::Connection &poseCon, util::Connection &transformCon)
			{
				poseCon.Disconnect();
				transformCon.Disconnect();
				delete aabb;
			}
		}

		// construct
		Aabb::Aabb(const phys::Form &form, bool pose) :
			Aabb(Bounds((assert(form.GetModel()), *form.GetModel()), pose), form) {}
		Aabb::Aabb(const Proxy<phys::Bounds> &bounds, const phys::attrib::Pose &pose) :
			bounds(bounds.Copy()), id(pose.GetId()) {}

		// clone
		Aabb *Aabb::Clone() const
		{
			return new Aabb(*this);
		}

		// attributes
		std::string Aabb::GetType() const
		{
			return "AABB";
		}
		std::string Aabb::GetSource() const
		{
			return bounds->GetSource() + ':' +
				util::lexical_cast<std::string>(id);
		}

		// dependency satisfaction
		Aabb::operator bool() const
		{
			return *bounds && util::Identifiable::FromId(id);
		}

		// instantiation
		Aabb::Instance Aabb::Make() const
		{
			phys::attrib::Pose &pose(util::ReferenceFromId<phys::attrib::Pose>(id));
			const phys::Bounds &bounds(**this->bounds);
			util::ScopedConnection
				poseCon(
					pose.IsPosed() && !bounds.bones.empty() ?
					pose.dirtyPoseSig.Connect(GetInvalidate()) :
					util::Connection()),
				transformCon(pose.dirtyTransformSig.Connect(GetInvalidate()));
			return Instance(new math::Aabb<3>(MakeAabb(bounds, pose)),
				std::bind(Delete, std::placeholders::_1,
					poseCon.Release(), transformCon.Release()));
		}
	}
}
