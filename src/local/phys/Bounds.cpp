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

#include <algorithm> // find, max, min

#include <boost/range/adaptor/indirected.hpp>

#include "../cache/proxy/Proxy.hpp" // Boost bug #5965
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
		Bounds(
			boost::adaptors::indirect(GetGeometricallyDistinctMeshes(model)),
			*model.skeleton) {}

	Bounds::Bounds(const boost::optional<const res::Skeleton &> &skeleton)
	{
		if (skeleton)
		{
			attrib::Pose pose(*skeleton);
			for (const auto &poseBone : pose.GetBones())
			{
				// FIXME: the non-static data-member initializers in the
				// definition of Bone prevented aggregate initialization
				Bone bone;
				bone.name      = poseBone.GetName();
				bone.origin    = poseBone.GetBindMatrix() * math::ZeroVector<3>();
				bone.direction = poseBone.GetBindMatrix() * math::NormVector<3>();
				bones.insert(std::make_pair(poseBone.GetName(), bone));
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
				bounds.staticBox = Max(bounds.staticBox, vertex.co);
			}
			else
			{
				// add vertex to bone bounding capsule
				auto iter(bounds.bones.find(*vertex.influences.front().bone));
				if (iter != bounds.bones.end())
				{
					auto &bone(iter->second);
					float
						mu = ClosestPointOnLineWeight(bone.origin, bone.direction, vertex.co),
						radius = Len(Lerp(bone.origin, bone.direction, mu) - vertex.co);
					bone.startWeight = std::min(bone.startWeight, mu);
					bone.endWeight   = std::max(bone.endWeight,   mu);
					bone.radius = std::max(bone.radius, radius);
				}
				else goto StaticVertex;
			}
		}
	}
}}
