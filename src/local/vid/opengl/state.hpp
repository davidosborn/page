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
