#ifndef    page_local_res_type_Gait_hpp
#   define page_local_res_type_Gait_hpp

#	include "../../cache/proxy/Resource.hpp"

namespace page
{
	namespace res
	{
		class Animation;

		struct Gait
		{
			struct State
			{
				cache::Resource<res::Animation> animation;
				float stride; // translation in metres, rotation in degrees
			};

			State run, sneak, turn, walk;
		};
	}
}

#endif
