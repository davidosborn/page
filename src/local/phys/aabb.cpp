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

#include "attrib/Pose.hpp" // Pose::{Bone,Get{Bone,Matrix}}
#include "Bounds.hpp"

namespace page
{
	namespace phys
	{
		math::Aabb<3> MakeAabb(const Bounds &bounds, const attrib::Pose &pose)
		{
			math::Aabb<3> aabb(bounds.staticBox);
			for (Bounds::Bones::const_iterator bone(bounds.bones.begin()); bone != bounds.bones.end(); ++bone)
				if (const attrib::Pose::Bone *poseBone = pose.GetBone(bone->name))
					for (unsigned i = 0; i < 2; ++i)
					{
						math::Vector<3> co(poseBone->GetPoseMatrix() * bone->co[i]);
						aabb = Max(Grow(math::Aabb<3>(co), bone->radius), aabb);
					}
			if (aabb != math::InverseInfiniteAabb<3>())
				aabb = pose.GetMatrix() * aabb;
			return aabb;
		}
	}
}
