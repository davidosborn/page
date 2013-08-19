#include <cassert>

#include <GL/gl.h>

#include "ext.hpp" // EXT_blend_func_separate
#include "get.hpp" // GetInteger

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			// separate blend functions
			void ColorBlendFunc(GLenum sfactor, GLenum dfactor)
			{
				assert(haveExtBlendFuncSeparate);
				glBlendFuncSeparateEXT(sfactor, dfactor,
					GetInteger(GL_BLEND_SRC_ALPHA_EXT),
					GetInteger(GL_BLEND_DST_ALPHA_EXT));
			}
			void AlphaBlendFunc(GLenum sfactor, GLenum dfactor)
			{
				assert(haveExtBlendFuncSeparate);
				glBlendFuncSeparateEXT(
					GetInteger(GL_BLEND_SRC_RGB_EXT),
					GetInteger(GL_BLEND_DST_RGB_EXT), sfactor, dfactor);
			}

			// separate clear functions
			void ClearColor(GLclampf red, GLclampf green, GLclampf blue)
			{
				GLfloat colorClearValue[4];
				glGetFloatv(GL_COLOR_CLEAR_VALUE, colorClearValue);
				glClearColor(red, green, blue, colorClearValue[3]);
			}
			void ClearAlpha(GLclampf alpha)
			{
				GLfloat colorClearValue[4];
				glGetFloatv(GL_COLOR_CLEAR_VALUE, colorClearValue);
				glClearColor(
					colorClearValue[0],
					colorClearValue[1],
					colorClearValue[2], alpha);
			}
		}
	}
}
