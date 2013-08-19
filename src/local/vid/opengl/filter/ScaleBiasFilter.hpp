#ifndef    page_local_vid_opengl_filter_ScaleBiasFilter_hpp
#   define page_local_vid_opengl_filter_ScaleBiasFilter_hpp

#	include "../Filter.hpp"

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			class RenderTarget;

			struct ScaleBiasFilter : Filter
			{
				// construct/destroy
				ScaleBiasFilter(DrawContext &, Resources &, float scale, float bias, bool opaque);
				~ScaleBiasFilter();

				// compatibility
				static bool Check(Resources &);

				private:
				float scale, bias;
			};
		}
	}
}

#endif
