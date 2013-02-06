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

#include <GL/gl.h>

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			GLboolean GetBoolean(GLenum value)
			{
				GLboolean data;
				glGetBooleanv(value, &data);
				return data;
			}
			GLdouble GetDouble(GLenum value)
			{
				GLdouble data;
				glGetDoublev(value, &data);
				return data;
			}
			GLfloat GetFloat(GLenum value)
			{
				GLfloat data;
				glGetFloatv(value, &data);
				return data;
			}
			GLint GetInteger(GLenum value)
			{
				GLint data;
				glGetIntegerv(value, &data);
				return data;
			}
			const char *GetString(GLenum value)
			{
				return reinterpret_cast<const char *>(glGetString(value));
			}
		}
	}
}
