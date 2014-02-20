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

#ifndef    page_local_phys_Bounds_hpp
#   define page_local_phys_Bounds_hpp

#	include <string>
#	include <unordered_map>
#	include <vector>

#	include <boost/optional.hpp>

#	include "../math/Aabb.hpp"
#	include "../math/float.hpp" // Inf
#	include "../math/Vector.hpp"
#	include "../util/type_traits/range.hpp" // is_range
#	include "../util/type_traits/sfinae.hpp" // ENABLE_IF

namespace page { namespace res
{
	class Mesh;
	class Model;
	class Skeleton;
}}

namespace page { namespace phys
{
	/**
	 * A bounding skeleton.  Each bone defines a capsule boundary that should
	 * enclose a mesh when attached to the associated skeleton.
	 */
	struct Bounds
	{
		/*-------------+
		| constructors |
		+-------------*/

		/**
		 * Creates a bounding skeleton using a model resource.  The model's
		 * skeleton and meshes will be used to initialize the bounding skeleton.
		 */
		explicit Bounds(const res::Model &);

		/**
		 * Creates a bounding skeleton using a range of mesh resources and an
		 * optional skeleton resource.
		 *
		 * @param[in] meshes A range of mesh resources.
		 * @param[in] skeleton An optional skeleton resource.
		 */
		template <typename MeshInputRange>
			explicit Bounds(
				MeshInputRange meshes,
				const boost::optional<const res::Skeleton &> & = nullptr,
				ENABLE_IF(util::is_range<MeshInputRange>::value));

		/**
		 * Creates a bounding skeleton using an optional skeleton resource.
		 */
		explicit Bounds(const boost::optional<const res::Skeleton &> & = nullptr);

		/*-------------+
		| data members |
		+-------------*/

		/**
		 * An AABB for static content.  Any meshes that do not have bone weights
		 * are assigned to this member.
		 */
		math::Aabb<3> staticBox = math::InverseInfiniteAabb<3>();

		/**
		 * A bounding bone.  A set of bones make up a bounding skeleton.  Each
		 * bone describes a capsule that can be used to generate an AABB for a
		 * specific pose.
		 */
		struct Bone
		{
			/**
			 * The bone's name.  The name is used to bind it to a pose.
			 */
			std::string name;

			/**
			 * The bone's origin.
			 *
			 * The @a origin and @a direction form a line that is used as a
			 * foundation on which to build the bounding capsule.  The end
			 * points of the capsule are positioned on the line according to
			 * @a startWeight and @a endWeight.
			 */
			math::Vec3 origin;

			/**
			 * The bone's direction.
			 *
			 * @sa origin
			 */
			math::Vec3 direction;

			/**
			 * A weight along the bone that determines the starting point of the
			 * capsule.
			 */
			float startWeight = 0;

			/**
			 * A weight along the bone that determines the ending point of the
			 * capsule.
			 */
			float endWeight = 1;

			/**
			 * The radius of the bounding capsule.
			 */
			float radius = 0;
		};

		/**
		 * The bones that make up the bounding skeleton.
		 */
		std::unordered_map<std::string, Bone> bones;
	};

	/**
	 * Adds a mesh to a bounding skeleton.
	 */
	void Add(Bounds &, const res::Mesh &);
}}

#	include "Bounds.tpp"
#endif
