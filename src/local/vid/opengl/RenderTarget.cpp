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
