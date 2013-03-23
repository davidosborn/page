/**
 * @section license
 *
 * Copyright (c) 2006-2013 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.

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

#pragma GCC system_header

#ifndef    page_system_GL_glu_hpp
#   define page_system_GL_glu_hpp

#	ifdef USE_WIN32
#		include <windows.h>
#	endif

#	include_next <GL/glu.h>

#	include "../../local/vid/opengl/state.hpp" // glu*_Shadow

#endif

#ifndef GLU_PURE
#	ifndef     page_extern_GL_glu_hpp_SHADOW
#	    define page_extern_GL_glu_hpp_SHADOW

#		define gluLookAt      page::vid::opengl::gluLookAt_Shadow
#		define gluOrtho2D     page::vid::opengl::gluOrtho2D_Shadow
#		define gluPerspective page::vid::opengl::gluPerspective_Shadow
#		define gluPickMatrix  page::vid::opengl::gluPickMatrix_Shadow
#		define gluProject     page::vid::opengl::gluProject_Shadow
#		define gluUnProject   page::vid::opengl::gluUnProject_Shadow

#	endif
#else
#	ifdef     page_extern_GL_glu_hpp_SHADOW
#	    undef page_extern_GL_glu_hpp_SHADOW

#		undef gluLookAt
#		undef gluOrtho2D
#		undef gluPerspective
#		undef gluPickMatrix
#		undef gluProject
#		undef gluUnProject

#	endif
#endif
