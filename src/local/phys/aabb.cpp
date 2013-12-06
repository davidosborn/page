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
