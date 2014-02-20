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

#include <boost/range/adaptor/map.hpp> // values

#include "attrib/Pose.hpp" // Pose::{Bone,Get{Bone,Matrix}}
#include "Bounds.hpp"

namespace page { namespace phys
{
	math::Aabb<3> MakeAabb(const Bounds &bounds, const attrib::Pose &pose)
	{
		math::Aabb<3> aabb(bounds.staticBox);
		for (const auto &bone : boost::adaptors::values(bounds.bones))
			if (const attrib::Pose::Bone *poseBone = pose.GetBone(bone.name))
			{
				math::Vec3
					a(poseBone->GetPoseMatrix() * (bone.origin * bone.startWeight)),
					b(poseBone->GetPoseMatrix() * (bone.direction * bone.endWeight));
				aabb = Max(Max(
					Grow(math::Aabb<3>(a), bone.radius),
					Grow(math::Aabb<3>(b), bone.radius)), aabb);
			}
		if (aabb != math::InverseInfiniteAabb<3>())
			aabb = pose.GetMatrix() * aabb;
		return aabb;
	}
}}
