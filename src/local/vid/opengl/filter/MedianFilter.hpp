#ifndef    page_local_vid_opengl_filter_MedianFilter_hpp
#   define page_local_vid_opengl_filter_MedianFilter_hpp

#	include "../Filter.hpp"

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			struct MedianFilter : Filter
			{
				// construct/destroy
				MedianFilter(DrawContext &, Resources &, unsigned passes, bool opaque);
				~MedianFilter();

				// compatibility
				static bool Check(Resources &);

				private:
				unsigned passes;
			};
		}
	}
}

#endif
