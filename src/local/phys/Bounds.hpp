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

#ifndef    page_local_phys_Bounds_hpp
#   define page_local_phys_Bounds_hpp

#	include <string>
#	include <unordered_map>
#	include <vector>
#	include "../math/Aabb.hpp"
#	include "../math/Vector.hpp"

namespace page
{
	namespace res
	{
		class Mesh;
		class Model;
		class Skeleton;
	}

	namespace phys
	{
		struct Bounds
		{
			// construct
			explicit Bounds(const res::Model &);
			explicit Bounds(const res::Mesh &);
			Bounds(
				const res::Mesh &,
				const res::Skeleton &);
			template <typename MeshIterator>
				Bounds(
					MeshIterator first,
					MeshIterator last);
			template <typename MeshIterator>
				Bounds(
					MeshIterator first,
					MeshIterator last,
					const res::Skeleton &);

			math::Aabb<3> staticBox;
			struct Bone
			{
				std::string name;
				math::Vector<3> co[2];
				float radius;
			};
			typedef std::vector<Bone> Bones;
			Bones bones;

			private:
			// initialization
			template <typename MeshIterator>
				void Init(
					MeshIterator first,
					MeshIterator last,
					const res::Skeleton * = 0);

			struct Scratch
			{
				struct Bone
				{
					math::Vector<3> co[2];
					float min, max, radius;
				};
				typedef std::unordered_map<std::string, Bone> Bones;
				Bones bones;
			};
			static void Init(Scratch &, const res::Skeleton &);
			void Extend(Scratch &, const res::Mesh &);
			void Apply(const Scratch &);
		};
	}
}

#	include "Bounds.tpp"
#endif
