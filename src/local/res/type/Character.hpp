#ifndef    page_local_res_type_Character_hpp
#   define page_local_res_type_Character_hpp

#	include <string>

#	include "../../cache/proxy/ResourceProxy.hpp"

namespace page
{
	namespace res
	{
		class Animation;
		class Gait;
		class Model;

		struct Character
		{
			// construct
			Character();

			std::string name;
			struct Animation
			{
				cache::Resource<res::Animation>
					ambient, cheer, clap, dance, jump, sleep;
			} animation;
			cache::ResourceProxy<Gait> gait;
			cache::ResourceProxy<Model> model;
			float radius, scale;
		};
	}
}

#endif
