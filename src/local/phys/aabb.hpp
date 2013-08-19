#ifndef    page_local_phys_aabb_hpp
#   define page_local_phys_aabb_hpp

#	include "../math/Aabb.hpp"

namespace page
{
	namespace phys
	{
		class Bounds;
		namespace attrib { class Pose; }

		math::Aabb<3> MakeAabb(const Bounds &, const attrib::Pose &);
	}
}

#endif
