#ifndef    page_local_res_type_Cursor_hpp
#   define page_local_res_type_Cursor_hpp

#	include "../../cache/proxy/Resource.hpp"
#	include "../../math/Vector.hpp"

namespace page
{
	namespace res
	{
		class Image;

		struct Cursor
		{
			cache::Resource<Image> image;
			math::Vec2 size, center;
		};
	}
}

#endif
