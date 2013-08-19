#include <cassert>

#include <GL/gl.h>

#include "ScaleBiasFilter.hpp"

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			// construct/destroy
			ScaleBiasFilter::ScaleBiasFilter(DrawContext &context, Resources &res, float scale, float bias, bool opaque) :
				Filter(context, res, opaque), scale(scale), bias(bias)
			{
				assert(Check(res));
			}
			ScaleBiasFilter::~ScaleBiasFilter()
			{
				PrepComposite();
				RestoreRenderTargetBinding();
				// FIXME: use scale/bias shader
				Fill(GetRenderTarget());
			}

			// compatibility
			bool ScaleBiasFilter::Check(Resources &res)
			{
				return Filter::Check(res);
			}
		}
	}
}
