#ifndef    page_local_res_type_Object_hpp
#   define page_local_res_type_Object_hpp

#	include <string>

#	include "../../cache/proxy/Resource.hpp"

namespace page
{
	namespace res
	{
		class Animation;
		class Model;

		struct Object
		{
			// construct
			Object();

			std::string name;
			struct Animation
			{
				cache::Resource<res::Animation> ambient;
			} animation;
			cache::Resource<Model> model;
			float radius, scale;
		};
	}
}

#endif
