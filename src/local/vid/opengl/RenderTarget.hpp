#ifndef    page_local_vid_opengl_RenderTarget_hpp
#   define page_local_vid_opengl_RenderTarget_hpp

#	include <GL/gl.h> // GL{enum,uint}

#	include "../../util/class/special_member_functions.hpp" // Uncopyable

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			class DrawContext;
			class Framebuffer;

			struct RenderTarget : util::Uncopyable<RenderTarget>
			{
				// construct
				RenderTarget(const Framebuffer &, GLenum);

				const Framebuffer &framebuffer;
				GLenum buffer;
			};

			// binding
			void Bind(const RenderTarget &);
			void Bind(const DrawContext &, const RenderTarget &);
			void ResetRenderTargetBinding(const DrawContext &);

			// texture access
			GLuint GetTexture(const RenderTarget &);
		}
	}
}

#endif
