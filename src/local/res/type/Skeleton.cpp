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

		REGISTER_TYPE(Skeleton, "skeleton")
	}
}
