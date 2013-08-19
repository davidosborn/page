#ifndef    page_local_vid_opengl_tex_hpp
#   define page_local_vid_opengl_tex_hpp

#	include <GL/gl.h> // GL{enum,{,u}int}

#	include "../../math/Vector.hpp"
#	include "../../res/type/Image.hpp" // Image::Channels

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			struct Compatibility
			{
				GLenum format, type;
				GLint internalFormat;
				res::Image::Channels channels;
			};
			Compatibility GetCompatibility(const res::Image &);

			GLuint MakeTexture(const res::Image &, bool mipmap = true, const math::Vector<2, bool> &clamp = false);
			GLuint MakeAlphaTexture(const res::Image &, bool mipmap = true, const math::Vector<2, bool> &clamp = false);
			GLuint MakeLuminanceTexture(const res::Image &, bool mipmap = true, const math::Vector<2, bool> &clamp = false);
		}
	}
}

#endif
