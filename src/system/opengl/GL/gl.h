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

#ifndef    page_system_GL_gl_hpp
#   define page_system_GL_gl_hpp

#	ifdef USE_WIN32
#		include <windows.h>
#	endif

#	include_next <GL/gl.h>

#	include "../../local/vid/opengl/state.hpp" // gl*_Shadow

#endif

#ifndef GL_PURE
#	ifndef     page_extern_GL_gl_hpp_SHADOW
#	    define page_extern_GL_gl_hpp_SHADOW

#		define glBlendFunc        page::vid::opengl::glBlendFunc_Shadow
#		define glDisable          page::vid::opengl::glDisable_Shadow
#		define glDrawBuffer       page::vid::opengl::glDrawBuffer_Shadow
#		define glEnable           page::vid::opengl::glEnable_Shadow
#		define glGetBooleanv      page::vid::opengl::glGetBooleanv_Shadow
#		define glGetDoublev       page::vid::opengl::glGetDoublev_Shadow
#		define glGetFloatv        page::vid::opengl::glGetFloatv_Shadow
#		define glGetIntegerv      page::vid::opengl::glGetIntegerv_Shadow
#		define glMatrixMode       page::vid::opengl::glMatrixMode_Shadow
#		define glPopAttrib        page::vid::opengl::glPopAttrib_Shadow
#		define glPopClientAttrib  page::vid::opengl::glPopClientAttrib_Shadow
#		define glPopMatrix        page::vid::opengl::glPopMatrix_Shadow
#		define glPushAttrib       page::vid::opengl::glPushAttrib_Shadow
#		define glPushClientAttrib page::vid::opengl::glPushClientAttrib_Shadow
#		define glPushMatrix       page::vid::opengl::glPushMatrix_Shadow
#		define glScissor          page::vid::opengl::glScissor_Shadow
#		define glViewport         page::vid::opengl::glViewport_Shadow

#	endif
#else
#	ifdef     page_extern_GL_gl_hpp_SHADOW
#	    undef page_extern_GL_gl_hpp_SHADOW

#		undef glBlendFunc
#		undef glDisable
#		undef glDrawBuffer
#		undef glEnable
#		undef glGetBooleanv
#		undef glGetDoublev
#		undef glGetFloatv
#		undef glGetIntegerv
#		undef glMatrixMode
#		undef glPopAttrib
#		undef glPopClientAttrib
#		undef glPopMatrix
#		undef glPushAttrib
#		undef glPushClientAttrib
#		undef glPushMatrix
#		undef glScissor
#		undef glViewport

#	endif
#endif
