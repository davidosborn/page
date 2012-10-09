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

// OpenGL state shadowing
// avoids driver read-back stalls

#ifndef    page_local_vid_opengl_state_hpp
#   define page_local_vid_opengl_state_hpp

#	include <GL/gl.h> // GL{boolean,double,enum,float,{,u}int,sizei}, glext.h
#	include <GL/glext.h> // GLhandleARB

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			// core functions
			void glBlendFunc_Shadow(GLenum, GLenum);
			void glDisable_Shadow(GLenum);
			void glDrawBuffer_Shadow(GLenum);
			void glEnable_Shadow(GLenum);
			void glGetBooleanv_Shadow(GLenum, GLboolean *);
			void glGetDoublev_Shadow(GLenum, GLdouble *);
			void glGetFloatv_Shadow(GLenum, GLfloat *);
			void glGetIntegerv_Shadow(GLenum, GLint *);
			void glMatrixMode_Shadow(GLenum);
			void glPopAttrib_Shadow();
			void glPopClientAttrib_Shadow();
			void glPopMatrix_Shadow();
			void glPushAttrib_Shadow(GLbitfield);
			void glPushClientAttrib_Shadow(GLbitfield);
			void glPushMatrix_Shadow();
			void glScissor_Shadow(GLint, GLint, GLsizei, GLsizei);
			void glViewport_Shadow(GLint, GLint, GLsizei, GLsizei);

			// utility functions
			void gluLookAt_Shadow(GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble, GLdouble);
			void gluOrtho2D_Shadow(GLdouble, GLdouble, GLdouble, GLdouble);
			void gluPerspective_Shadow(GLdouble, GLdouble, GLdouble, GLdouble);
			void gluPickMatrix_Shadow(GLdouble, GLdouble, GLdouble, GLdouble, GLint[4]);
			int gluProject_Shadow(GLdouble, GLdouble, GLdouble, const GLdouble[16], const GLdouble[16], const GLint[4], GLdouble *, GLdouble *, GLdouble *);
			int gluUnProject_Shadow(GLdouble, GLdouble, GLdouble, const GLdouble[16], const GLdouble[16], const GLint[4], GLdouble *, GLdouble *, GLdouble *);

			// extension functions
			// ARB_multitexture
			void glActiveTextureARB_Shadow(GLenum);
			void glClientActiveTextureARB_Shadow(GLenum);
			// ARB_shader_objects
			GLhandleARB glGetHandleARB_Shadow(GLenum);
			void glUseProgramObjectARB_Shadow(GLhandleARB);
			// EXT_blend_func_separate
			void glBlendFuncSeparateEXT_Shadow(GLenum, GLenum, GLenum, GLenum);
			// EXT_framebuffer_object
			void glBindFramebufferEXT_Shadow(GLenum, GLuint);
		}
	}
}

#endif
