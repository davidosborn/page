#ifndef    page_local_res_load_font_freetype_hpp
#   define page_local_res_load_font_freetype_hpp

#	include "../../adapt/freetype.hpp" // OpenArgs

namespace page
{
	namespace res
	{
		class Font;

		Font *LoadFreetypeFont(const OpenArgs &args, unsigned faceIndex = 0);
	}
}

#endif
