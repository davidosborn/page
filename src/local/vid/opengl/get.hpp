#ifndef    page_local_vid_opengl_get_hpp
#   define page_local_vid_opengl_get_hpp

#	include <GL/gl.h> // GL{boolean,double,enum,float,int}

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			GLboolean GetBoolean(GLenum);
			GLdouble GetDouble(GLenum);
			GLfloat GetFloat(GLenum);
			GLint GetInteger(GLenum);
			const char *GetString(GLenum);
		}
	}
}

#endif
