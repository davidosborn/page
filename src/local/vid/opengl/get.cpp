#include <GL/gl.h>

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			GLboolean GetBoolean(GLenum value)
			{
				GLboolean data;
				glGetBooleanv(value, &data);
				return data;
			}
			GLdouble GetDouble(GLenum value)
			{
				GLdouble data;
				glGetDoublev(value, &data);
				return data;
			}
			GLfloat GetFloat(GLenum value)
			{
				GLfloat data;
				glGetFloatv(value, &data);
				return data;
			}
			GLint GetInteger(GLenum value)
			{
				GLint data;
				glGetIntegerv(value, &data);
				return data;
			}
			const char *GetString(GLenum value)
			{
				return reinterpret_cast<const char *>(glGetString(value));
			}
		}
	}
}
