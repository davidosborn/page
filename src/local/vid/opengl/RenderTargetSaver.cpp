#include "ext.hpp" // EXT_framebuffer_object
#include "get.hpp" // Get{Boolean,Integer}
#include "RenderTargetSaver.hpp"

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			// construct
			RenderTargetSaver::RenderTargetSaver(bool enabled)
			{
				Reset(enabled);
			}

			// modifiers
			void RenderTargetSaver::Save()
			{
				if (haveExtFramebufferObject)
					framebuffer = GetInteger(GL_FRAMEBUFFER_BINDING_EXT);
				drawBuffer = GetInteger(GL_DRAW_BUFFER);
				glGetIntegerv(GL_SCISSOR_BOX, scissorBox);
				glGetIntegerv(GL_VIEWPORT, viewport);
			}
			void RenderTargetSaver::Load()
			{
				if (haveExtFramebufferObject)
					glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebuffer);
				glDrawBuffer(drawBuffer);
				glViewport(
					viewport[0], viewport[1],
					viewport[2], viewport[3]);
				glScissor(
					scissorBox[0], scissorBox[1],
					scissorBox[2], scissorBox[3]);
			}
		}
	}
}
