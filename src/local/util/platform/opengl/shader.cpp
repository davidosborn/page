/**
 * @copyright
 *
 * Copyright (c) 2006-2014 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the same place
 *    and form as other copyright, license, and disclaimer information.
 *
 * 3. Redistributions in binary form must also include an acknowledgement in the
 *    same place and form as other acknowledgements (such as the credits),
 *    similar in substance to the following:
 *
 *       Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#include <string>

#include <GL/gl.h> // GLenum
#include <GL/glext.h> // GL_*_ARB

namespace page
{
	namespace util
	{
		namespace opengl
		{
			// uniform types
			std::string GetUniformTypeName(GLenum type)
			{
				switch (type)
				{
					case GL_FLOAT:                      return "float";
					case GL_FLOAT_VEC2_ARB:             return "vec2";
					case GL_FLOAT_VEC3_ARB:             return "vec3";
					case GL_FLOAT_VEC4_ARB:             return "vec4";
					case GL_INT:                        return "int";
					case GL_INT_VEC2_ARB:               return "ivec2";
					case GL_INT_VEC3_ARB:               return "ivec3";
					case GL_INT_VEC4_ARB:               return "ivec4";
					case GL_BOOL_ARB:                   return "bool";
					case GL_BOOL_VEC2_ARB:              return "bvec2";
					case GL_BOOL_VEC3_ARB:              return "bvec3";
					case GL_BOOL_VEC4_ARB:              return "bvec4";
					case GL_FLOAT_MAT2_ARB:             return "mat2";
					case GL_FLOAT_MAT3_ARB:             return "mat3";
					case GL_FLOAT_MAT4_ARB:             return "mat4";
					case GL_SAMPLER_1D_ARB:             return "sampler1D";
					case GL_SAMPLER_2D_ARB:             return "sampler2D";
					case GL_SAMPLER_3D_ARB:             return "sampler3D";
					case GL_SAMPLER_CUBE_ARB:           return "samplerCube";
					case GL_SAMPLER_1D_SHADOW_ARB:      return "sampler1DShadow";
					case GL_SAMPLER_2D_SHADOW_ARB:      return "sampler2DShadow";
					case GL_SAMPLER_2D_RECT_ARB:        return "sampler2DRect";
					case GL_SAMPLER_2D_RECT_SHADOW_ARB: return "sampler2DRectShadow";
					default:                            return "unknown";
				}
			}
		}
	}
}
