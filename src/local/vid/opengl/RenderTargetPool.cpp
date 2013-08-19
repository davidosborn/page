#include <cassert>
#include <functional> // bind

#include "RenderTarget.hpp"
#include "RenderTargetPool.hpp"

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			// construct
			RenderTargetPool::RenderTargetPool(const math::Vec2u &size, GLenum format, unsigned colorAttachments, Flags flags) :
				framebuffer(size, format, flags)
			{
				assert(colorAttachments);
				for (unsigned i = 0; i < colorAttachments; ++i)
					freeColorAttachments.push(framebuffer.AttachColorTexture());
			}

			// framebuffer attributes
			const math::Vec2u &RenderTargetPool::GetSize() const
			{
				return framebuffer.GetSize();
			}
			const math::Vec2u &RenderTargetPool::GetPow2Size() const
			{
				return framebuffer.GetPow2Size();
			}

			// factory
			RenderTargetPool::Pooled RenderTargetPool::Get() const
			{
				if (freeColorAttachments.empty())
					freeColorAttachments.push(framebuffer.AttachColorTexture());
				Pooled result(
					new RenderTarget(framebuffer, freeColorAttachments.front()),
					std::bind(
						&RenderTargetPool::DeleteRenderTarget,
						this, std::placeholders::_1));
				freeColorAttachments.pop();
				return result;
			}

			// deleter
			void RenderTargetPool::DeleteRenderTarget(const RenderTarget *rt) const
			{
				freeColorAttachments.push(rt->buffer);
				delete rt;
			}

			// binding
			void Bind(const RenderTargetPool::Pooled &rt)
			{
				assert(rt);
				Bind(*rt);
			}
			void Bind(const DrawContext &dc, const RenderTargetPool::Pooled &rt)
			{
				assert(rt);
				Bind(dc, *rt);
			}

			// texture access
			GLuint GetTexture(const RenderTargetPool::Pooled &rt)
			{
				assert(rt);
				return GetTexture(*rt);
			}
		}
	}
}
