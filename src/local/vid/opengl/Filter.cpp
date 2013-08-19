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
