#include <algorithm> // find, max, min

#include "../math/interp.hpp" // Lerp
#include "../math/intersect.hpp" // ClosestPointOnLineWeight
#include "../math/Matrix.hpp"
#include "../res/type/Mesh.hpp"
#include "../res/type/Model.hpp"
#include "attrib/Pose.hpp"
#include "Bounds.hpp"

namespace page { namespace phys
{
	/*-------------+
	| constructors |
	+-------------*/

	Bounds::Bounds(const res::Model &model) :
		Bounds(GetGeometricallyDistinctMeshes(model), model.skeleton) {}

	Bounds::Bounds(const boost::optional<const res::Skeleton &> &skeleton)
	{
		if (skeleton)
		{
			attrib::Pose pose(*skeleton);
			for (attrib::Pose::ConstBoneIterator poseBone(pose.BeginBones()); poseBone != pose.EndBones(); ++poseBone)
			{
				Bone bone =
				{
					poseBone->GetName(),
					poseBone->GetBindMatrix() * math::ZeroVector<3>(),
					poseBone->GetBindMatrix() * math::NormVector<3>()
				};
				bones.insert(std::make_pair(poseBone->GetName(), bone));
			}
		}
	}

	void Add(Bounds &bounds, const res::Mesh &mesh)
	{
		for (const auto &vertex : mesh.vertices)
		{
			if (vertex.influences.empty())
			{
				StaticVertex:
				// add vertex to static bounding box
				staticBox = Max(staticBox, vertex.co);
			}
			else
			{
				// add vertex to bone bounding capsule
				auto iter(bounds.bones.find(*vertex.influences.front().bone));
				if (iter != bounds.bones.end())
				{
					auto &bone(iter->second);
					float
						mu = ClosestPointOnLineWeight(bone.start, bone.end, vertex.co),
						radius = Len(Lerp(bone.start, bone.end, mu) - vertex.co);
					bone.startWeight = std::min(bone.startWeight, mu);
					bone.endWeight   = std::max(bone.endWeight,   mu);
					bone.radius = std::max(bone.radius, radius);
				}
				else goto StaticVertex;
			}
		}
	}
}}
