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
