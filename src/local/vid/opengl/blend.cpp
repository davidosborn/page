/**
 * @section license
 *
 * Copyright (c) 2006-2012 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution, and in the same
 *    place and form as other copyright, license, and disclaimer information.
 *
 * As a special exception, distributions of derivative works in binary form may
 * include an acknowledgement in place of the above copyright notice, this list
 * of conditions, and the following disclaimer in the documentation and/or other
 * materials provided with the distribution, and in the same place and form as
 * other acknowledgements, similar in substance to the following:
 *
 *    Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

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
