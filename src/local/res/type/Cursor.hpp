#ifndef    page_local_res_type_Cursor_hpp
#   define page_local_res_type_Cursor_hpp

#	include "../../cache/proxy/ResourceProxy.hpp"
#	include "../../math/Vector.hpp"

namespace page
{
	namespace res
	{
		class Image;

		struct Cursor
		{
			cache::ResourceProxy<Image> image;
			math::Vec2 size, center;
		};
	}
}

#endif
