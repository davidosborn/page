#ifndef    page_local_util_platform_opengl_shader_hpp
#   define page_local_util_platform_opengl_shader_hpp

#	include <string>

#	include <GL/gl.h> // GLenum

namespace page
{
	namespace util
	{
		namespace opengl
		{
			// uniform types
			std::string GetUniformTypeName(GLenum);
		}
	}
}

#endif
