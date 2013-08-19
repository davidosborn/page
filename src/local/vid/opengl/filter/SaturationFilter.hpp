#ifndef    page_local_vid_opengl_filter_SaturationFilter_hpp
#   define page_local_vid_opengl_filter_SaturationFilter_hpp

#	include "../Filter.hpp"

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			class RenderTarget;

			struct SaturationFilter : Filter
			{
				// construct/destroy
				SaturationFilter(DrawContext &, Resources &, float saturation, bool opaque);
				~SaturationFilter();

				// compatibility
				static bool Check(Resources &);

				private:
				float saturation;
			};
		}
	}
}

#endif
