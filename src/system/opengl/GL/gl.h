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
