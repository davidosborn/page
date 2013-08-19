#ifndef    page_local_err_platform_opengl_hpp
#   define page_local_err_platform_opengl_hpp

#	include <string>

#	include <GL/gl.h> // GLenum

namespace page { namespace err { namespace opengl
{
	void ResetError();
	void CheckError();
	std::string GetErrorString(GLenum error);
}}}

#endif
