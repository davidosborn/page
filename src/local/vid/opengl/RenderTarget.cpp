/**
 * @copyright
 *
 * Copyright (c) 2006-2014 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the same place
 *    and form as other copyright, license, and disclaimer information.
 *
 * 3. Redistributions in binary form must also include an acknowledgement in the
 *    same place and form as other acknowledgements (such as the credits),
 *    similar in substance to the following:
 *
 *       Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#include "../../math/Aabb.hpp"
#include "../../wnd/Window.hpp" // Window::GetSize
#include "DrawContext.hpp" // DrawContext::Get{Clip,Driver,LogicalBox}
#include "Driver.hpp" // Driver::GetWindow
#include "Framebuffer.hpp" // Bind, Framebuffer::Get{Size,Texture}
#include "RenderTarget.hpp"

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			namespace
			{
				void SetScissor(const math::Vec2u &size)
				{
					glScissor(0, 0, size.x, size.y);
				}
				void SetScissor(const DrawContext &dc, const math::Vec2u &size)
				{
					math::Aabb<2, unsigned> scissor(
						Ceil(LeaveSpace(dc.GetLogicalBox(), dc.GetClip()) * size));
					glScissor(
						scissor.min.x,
						size.y - scissor.max.y,
						scissor.max.x - scissor.min.x,
						scissor.max.y - scissor.min.y);
				}
				void SetScissor(const DrawContext &dc)
				{
					SetScissor(dc, dc.GetDriver().GetWindow().GetSize());
				}
			}

			// construct
			RenderTarget::RenderTarget(const Framebuffer &framebuffer, GLenum buffer) :
				framebuffer(framebuffer), buffer(buffer) {}

			// binding
			void Bind(const RenderTarget &rt)
			{
				Bind(rt.framebuffer);
				glDrawBuffer(rt.buffer);
				SetScissor(rt.framebuffer.GetSize());
			}
			void Bind(const DrawContext &dc, const RenderTarget &rt)
			{
				Bind(rt.framebuffer);
				glDrawBuffer(rt.buffer);
				SetScissor(dc, rt.framebuffer.GetSize());
			}
			void ResetRenderTargetBinding(const DrawContext &dc)
			{
				ResetFramebufferBinding(dc);
				glDrawBuffer(GL_BACK);
				SetScissor(dc);
			}

			// texture access
			GLuint GetTexture(const RenderTarget &rt)
			{
				rt.framebuffer.GetTexture(rt.buffer);
			}
		}
	}
}
