#include <cassert>

#include <GL/gl.h>

#include "../blend.hpp" // ColorBlendFunc
#include "AlphaFilter.hpp"

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			// construct/destroy
			AlphaFilter::AlphaFilter(DrawContext &context, Resources &res, float opacity, bool opaque) :
				Filter(context, res, opaque), opacity(opacity)
			{
				assert(Check(res));
			}
			AlphaFilter::~AlphaFilter()
			{
				PrepComposite();
				RestoreRenderTargetBinding();
				ColorBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glEnable(GL_BLEND);
				glPushAttrib(GL_CURRENT_BIT);
				glColor4f(1, 1, 1, opacity);
				Fill(GetRenderTarget());
			}

			// compatibility
			bool AlphaFilter::Check(Resources &res)
			{
				return Filter::Check(res);
			}
		}
	}
}
