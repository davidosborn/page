#ifndef    page_local_vid_opengl_filter_GlowFilter_hpp
#   define page_local_vid_opengl_filter_GlowFilter_hpp

#	include "../Filter.hpp"

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			struct GlowFilter : Filter
			{
				// construct/destroy
				GlowFilter(DrawContext &, Resources &, float intensity, bool opaque);
				~GlowFilter();

				// compatibility
				static bool Check(Resources &);

				private:
				float intensity;
			};
		}
	}
}

#endif
