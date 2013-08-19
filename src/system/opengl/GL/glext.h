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
