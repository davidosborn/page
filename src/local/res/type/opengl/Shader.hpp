#ifndef    page_local_res_type_opengl_Shader_hpp
#   define page_local_res_type_opengl_Shader_hpp

#	include <string>

#	include <GL/gl.h> // GLenum

namespace page
{
	namespace res
	{
		namespace opengl
		{
			struct Shader
			{
				GLenum type;
				std::string source;
			};
		}
	}
}

#endif
