#ifndef    page_local_vid_opengl_filter_AlphaFilter_hpp
#   define page_local_vid_opengl_filter_AlphaFilter_hpp

#	include "../Filter.hpp"

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			class RenderTarget;

			struct AlphaFilter : Filter
			{
				// construct/destroy
				AlphaFilter(DrawContext &, Resources &, float opacity, bool opaque);
				~AlphaFilter();

				// compatibility
				static bool Check(Resources &);

				private:
				float opacity;
			};
		}
	}
}

#endif
