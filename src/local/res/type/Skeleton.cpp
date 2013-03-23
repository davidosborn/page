/**
 * @section license
 *
 * Copyright (c) 2006-2013 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
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

#include "Registry.hpp" // REGISTER_TYPE
#include "Skeleton.hpp"

namespace page
{
	namespace res
	{
		Skeleton::Skeleton() {}
		Skeleton::Skeleton(const Skeleton &other) : bones(other.bones)
		{
			// remap parent pointers
			Bones::const_iterator otherBone(other.bones.begin());
			for (Bones::iterator bone(bones.begin()); bone != bones.end(); ++bone, ++otherBone)
				bone->parent = &*bones.begin() + (otherBone - other.bones.begin());
		}
		Skeleton &Skeleton::operator =(const Skeleton &other)
		{
			bones = other.bones;
			// remap parent pointers
			Bones::const_iterator otherBone(other.bones.begin());
			for (Bones::iterator bone(bones.begin()); bone != bones.end(); ++bone, ++otherBone)
				bone->parent = &*bones.begin() + (otherBone - other.bones.begin());
		}

		REGISTER_TYPE(Skeleton, "skeleton", 0)
	}
}
