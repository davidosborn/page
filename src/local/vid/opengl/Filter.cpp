/**
 * @section license
 *
 * Copyright (c) 2006-2013 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution, and in the same
 *    place and form as other copyright, license, and disclaimer information.
 *
 * As a special exception, distributions of derivative works in binary form may
 * include an acknowledgement in place of the above copyright notice, this list
 * of conditions, and the following disclaimer in the documentation and/or other
 * materials provided with the distribution, and in the same place and form as
 * other acknowledgements, similar in substance to the following:
 *
 *    Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#include <GL/gl.h>

#include "blend.hpp" // {Alpha,Color}BlendFunc
#include "DrawContext.hpp" // DrawContext->vid::DrawContext
#include "ext.hpp" // EXT_blend_func_separate
#include "Filter.hpp"
#include "get.hpp" // GetBoolean
#include "Resources.hpp" // Resources::{{Get,Has}RenderTargetPool,rgb{,a}CompositeRenderTargetPool}

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			// construct
			Filter::Filter(DrawContext &context, Resources &res, bool opaque) :
				vid::Filter(context), res(res), opaque(opaque)
			{
				assert(Check(res));
				glPushAttrib(GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT);
				Bind(GetContext(), renderTarget = res.GetRenderTargetPool(
					opaque ?
						Resources::rgbCompositeRenderTargetPool :
						Resources::rgbaCompositeRenderTargetPool).Get());
				if (!opaque)
				{
					AlphaBlendFunc(GL_ONE, GL_ONE);
					glClearColor(.5, .5, .5, 0);
				}
				GLbitfield buffer = GL_COLOR_BUFFER_BIT;
				if (GetBoolean(GL_DEPTH_TEST)) buffer |= GL_DEPTH_BUFFER_BIT;
				glClear(buffer);
			}

			// context access
			DrawContext &Filter::GetContext()
			{
				return dynamic_cast<DrawContext &>(vid::Filter::GetContext());
			}
			const DrawContext &Filter::GetContext() const
			{
				return dynamic_cast<const DrawContext &>(vid::Filter::GetContext());
			}

			// resource access
			Resources &Filter::GetResources()
			{
				return res;
			}
			const Resources &Filter::GetResources() const
			{
				return res;
			}

			// attributes
			bool Filter::IsOpaque() const
			{
				return opaque;
			}

			// render-target access
			const RenderTarget &Filter::GetRenderTarget() const
			{
				return *renderTarget;
			}

			// render-target binding modifiers
			void Filter::RestoreRenderTargetBinding()
			{
				renderTargetSaver.Restore();
			}

			// rendering
			void Filter::PrepComposite()
			{
				glDisable(GL_LIGHTING);
				glDisable(GL_ALPHA_TEST);
				glDisable(GL_DEPTH_TEST);
				if (!opaque)
				{
					ColorBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					glEnable(GL_BLEND);
				}
				else glDisable(GL_BLEND);
			}
			void Filter::Fill(const RenderTarget &rt, float margin)
			{
				// FIXME: fit fill-box around rendered geometry and margin
				GetContext().Fill(rt);
			}

			// compatibility
			bool Filter::Check(Resources &res)
			{
				return
					res.HasRenderTargetPool(Resources::rgbCompositeRenderTargetPool) &&
					res.HasRenderTargetPool(Resources::rgbaCompositeRenderTargetPool) &&
					haveExtBlendFuncSeparate;
			}
		}
	}
}
