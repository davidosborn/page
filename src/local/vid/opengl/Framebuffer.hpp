#ifndef    page_local_vid_opengl_Framebuffer_hpp
#   define page_local_vid_opengl_Framebuffer_hpp

#	include <vector>

#	include <GL/gl.h> // GL{enum,uint}

#	include "../../math/Vector.hpp"
#	include "../../util/class/special_member_functions.hpp" // Uncopyable
#	include "FramebufferFlags.hpp"

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			class DrawContext;

			struct Framebuffer : util::Uncopyable<Framebuffer>
			{
				typedef FramebufferFlags Flags;

				// construct/destroy
				Framebuffer(const math::Vec2u &size,
					GLenum colorFormat, Flags = noneFramebufferFlag);
				~Framebuffer();

				// attributes
				const math::Vec2u &GetSize() const;
				const math::Vec2u &GetPow2Size() const;

				// handle access
				GLuint GetHandle() const;
				GLuint GetRenderbuffer() const;
				GLuint GetTexture(GLenum attachment) const;

				// attachment modifiers
				GLenum AttachColorTexture();

				private:
				// completeness
				void Verify() const;
				static bool CheckStatus();

				math::Vec2u size, pow2Size;
				GLuint framebuffer, renderbuffer;
				std::vector<GLuint> colorTextures;
				GLenum colorFormat;
				Flags flags;
			};

			// binding
			void Bind(const Framebuffer &);
			void ResetFramebufferBinding(const DrawContext &);
		}
	}
}

#endif
