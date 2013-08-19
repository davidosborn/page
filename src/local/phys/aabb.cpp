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
						math::Vec3 co(poseBone->GetPoseMatrix() * bone->co[i]);
						aabb = Max(Grow(math::Aabb<3>(co), bone->radius), aabb);
					}
			if (aabb != math::InverseInfiniteAabb<3>())
				aabb = pose.GetMatrix() * aabb;
			return aabb;
		}
	}
}
