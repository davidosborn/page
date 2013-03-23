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

#include <algorithm> // find, max, min, transform, unique
#include <functional> // mem_fn
#include <iterator> // back_inserter

#include "../math/float.hpp" // Inf
#include "../math/interp.hpp" // Lerp
#include "../math/intersect.hpp" // ClosestPointOnLineWeight
#include "../math/Matrix.hpp"
#include "../res/type/Mesh.hpp"
#include "../res/type/Model.hpp"
#include "../util/functional/pointer.hpp" // dereference
#include "../util/iterator/indirect_iterator.hpp"
#include "../util/iterator/member_iterator.hpp"
#include "../util/raii/copy_ptr.hpp"
#include "attrib/Pose.hpp"
#include "Bounds.hpp"

namespace page
{
	namespace phys
	{
		// construct
		Bounds::Bounds(const res::Model &model)
		{
			assert(std::find(
				util::make_member_iterator(model.parts.begin(), &res::Model::Part::mesh),
				util::make_member_iterator(model.parts.end(),   &res::Model::Part::mesh),
				false).base() == model.parts.end());
			// remove duplicate meshes
			// FIXME: this could be slower than processing the duplicate meshes
			typedef std::vector<util::copy_ptr<cache::Proxy<res::Mesh>>> Meshes;
			Meshes meshes;
			std::transform(
				util::make_member_iterator(model.parts.begin(), &res::Model::Part::mesh),
				util::make_member_iterator(model.parts.end(),   &res::Model::Part::mesh),
				std::back_inserter(meshes),
				std::mem_fn(&cache::Proxy<res::Mesh>::Copy));
			using namespace std::placeholders;
			meshes.erase(
				std::unique(meshes.begin(), meshes.end(),
					bind(cache::CompareProxySource<res::Mesh>(),
						bind(util::dereference<util::copy_ptr<cache::Proxy<res::Mesh>>>(), _1),
						bind(util::dereference<util::copy_ptr<cache::Proxy<res::Mesh>>>(), _2))),
				meshes.end());
			Init(
				util::make_indirect_iterator(util::make_indirect_iterator(meshes.begin())),
				util::make_indirect_iterator(util::make_indirect_iterator(meshes.end())),
				model.skeleton ? &*model.skeleton : 0);
		}
		Bounds::Bounds(const res::Mesh &mesh)
		{
			Init(&mesh, &mesh + 1);
		}
		Bounds::Bounds(const res::Mesh &mesh, const res::Skeleton &skel)
		{
			Init(&mesh, &mesh + 1, &skel);
		}

		// initialization
		void Bounds::Init(Scratch &scratch, const res::Skeleton &skel)
		{
			attrib::Pose pose(skel);
			for (attrib::Pose::ConstBoneIterator poseBone(pose.BeginBones()); poseBone != pose.EndBones(); ++poseBone)
			{
				Scratch::Bone bone =
				{
					poseBone->GetBindMatrix() * math::ZeroVector<3>(),
					poseBone->GetBindMatrix() * math::NormVector<3>(),
					 math::Inf<float>(), // min
					-math::Inf<float>(), // max
					-1                   // radius
				};
				scratch.bones.insert(std::make_pair(poseBone->GetName(), bone));
			}
		}
		void Bounds::Extend(Scratch &scratch, const res::Mesh &mesh)
		{
			for (res::Mesh::Vertices::const_iterator vertex(mesh.vertices.begin()); vertex != mesh.vertices.end(); ++vertex)
			{
				if (vertex->influences.empty())
				{
					StaticVertex:
					// add vertex to static bounding box
					staticBox = Max(staticBox, vertex->co);
				}
				else
				{
					// add vertex to bone bounding capsule
					Scratch::Bones::iterator iter(scratch.bones.find(*vertex->influences.front().bone));
					if (iter != scratch.bones.end())
					{
						Scratch::Bone &bone(iter->second);
						float
							mu = ClosestPointOnLineWeight(bone.co[0], bone.co[1], vertex->co),
							radius = Len(Lerp(bone.co[0], bone.co[1], mu) - vertex->co);
						bone.min = std::min(bone.min, mu - radius);
						bone.max = std::max(bone.max, mu + radius);
						bone.radius = std::max(bone.radius, radius);
					}
					else goto StaticVertex;
				}
			}
		}
		void Bounds::Apply(const Scratch &scratch)
		{
			bones.reserve(scratch.bones.size());
			for (Scratch::Bones::const_iterator iter(scratch.bones.begin()); iter != scratch.bones.end(); ++iter)
			{
				const Scratch::Bone &scratchBone(iter->second);
				if (scratchBone.radius != -1)
				{
					Bone bone =
					{
						iter->first, // name
						math::NormVector<3>() * (scratchBone.min + scratchBone.radius),
						math::NormVector<3>() * (scratchBone.max - scratchBone.radius),
						scratchBone.radius
					};
					bones.push_back(bone);
				}
			}
		}
	}
}
