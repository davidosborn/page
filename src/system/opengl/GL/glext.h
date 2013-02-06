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

#ifndef    page_system_GL_glext_hpp
#   define page_system_GL_glext_hpp

	// FIXME: should we disable these versions on other platforms as well?
#	ifdef USE_WIN32
		// inhibit OpenGL version > 1.1
#		define GL_VERSION_1_2 1
#		define GL_VERSION_1_3 1
#		define GL_VERSION_1_4 1
#		define GL_VERSION_1_5 1
#		define GL_VERSION_2_0 1
#	endif

#	ifdef USE_WIN32
#		include <windows.h>
#	endif

#	include_next <GL/glext.h>

#	include "../../local/vid/opengl/state.hpp" // gl*{ARB,EXT}_Shadow

#endif

#ifndef GLEXT_PURE
#	ifndef     page_extern_GL_glext_hpp_SHADOW
#	    define page_extern_GL_glext_hpp_SHADOW

		// ARB_multitexture
#		define glActiveTextureARB       page::vid::opengl::glActiveTextureARB_Shadow
#		define glClientActiveTextureARB page::vid::opengl::glClientActiveTextureARB_Shadow
		// ARB_shader_objects
#		define glGetHandleARB           page::vid::opengl::glGetHandleARB_Shadow
#		define glUseProgramObjectARB    page::vid::opengl::glUseProgramObjectARB_Shadow
		// EXT_blend_func_separate
#		define glBlendFuncSeparateEXT   page::vid::opengl::glBlendFuncSeparateEXT_Shadow
		// EXT_framebuffer_object
#		define glBindFramebufferEXT     page::vid::opengl::glBindFramebufferEXT_Shadow

#	endif
#else
#	ifdef     page_extern_GL_glext_hpp_SHADOW
#	    undef page_extern_GL_glext_hpp_SHADOW

		// ARB_multitexture
#		undef glActiveTextureARB
#		undef glClientActiveTextureARB
		// ARB_shader_objects
#		undef glGetHandleARB
#		undef glUseProgramObjectARB
		// EXT_blend_func_separate
#		undef glBlendFuncSeparateEXT
		// EXT_framebuffer_object
#		undef glBindFramebufferEXT

#	endif
#endif
