#ifndef    page_local_vid_opengl_RenderTargetPool_hpp
#   define page_local_vid_opengl_RenderTargetPool_hpp

#	include <memory> // shared_ptr
#	include <queue>

#	include <GL/gl.h> // GL{enum,uint}

#	include "../../math/fwd.hpp" // Vector
#	include "../../util/class/special_member_functions.hpp" // Uncopyable
#	include "Framebuffer.hpp"
#	include "FramebufferFlags.hpp"

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			class DrawContext;
			class RenderTarget;

			struct RenderTargetPool : util::Uncopyable<RenderTargetPool>
			{
				typedef FramebufferFlags Flags;
				typedef std::shared_ptr<const RenderTarget> Pooled;

				// construct
				RenderTargetPool(const math::Vec2u &size,
					GLenum format, unsigned colorAttachments = 1,
					Flags = noneFramebufferFlag);

				// framebuffer attributes
				const math::Vec2u &GetSize() const;
				const math::Vec2u &GetPow2Size() const;

				// factory
				Pooled Get() const;

				private:
				// deleter
				void DeleteRenderTarget(const RenderTarget *) const;

				mutable Framebuffer framebuffer;
				mutable std::queue<GLenum> freeColorAttachments;
			};

			// binding
			void Bind(const RenderTargetPool::Pooled &);
			void Bind(const DrawContext &, const RenderTargetPool::Pooled &);

			// texture access
			GLuint GetTexture(const RenderTargetPool::Pooled &);
		}
	}
}

#endif
