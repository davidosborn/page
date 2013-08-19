#ifndef    page_local_res_type_Skeleton_hpp
#   define page_local_res_type_Skeleton_hpp

#	include <string>
#	include <vector>

#	include "../../math/Quat.hpp"
#	include "../../math/Vector.hpp"

namespace page
{
	namespace res
	{
		struct Skeleton
		{
			Skeleton();
			Skeleton(const Skeleton &);
			Skeleton &operator =(const Skeleton &);

			struct Bone
			{
				std::string name;
				math::Vec3 position;
				math::Quat<> orientation;
				math::Vec3 scale;
				const Bone *parent;
			};
			typedef std::vector<Bone> Bones;
			Bones bones;
		};
	}
}

#endif
