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
		 */
		template <typename MeshInputRange>
			explicit Bounds(
				MeshInputRange,
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
