#ifndef    page_local_vid_opengl_blend_hpp
#   define page_local_vid_opengl_blend_hpp

#	include <GL/gl.h> // GL{clampf,enum}

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			// separate blend functions
			// requires EXT_blend_func_separate
			void ColorBlendFunc(GLenum sfactor, GLenum dfactor);
			void AlphaBlendFunc(GLenum sfactor, GLenum dfactor);

			// separate clear functions
			void ClearColor(GLclampf red, GLclampf green, GLclampf blue);
			void ClearAlpha(GLclampf);
		}
	}
}

#endif
