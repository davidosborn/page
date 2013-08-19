#include <string>

#include <GL/gl.h> // GLenum, glGetError
#ifdef HAVE_GLU
#	include <GL/glu.h> // gluErrorString
#endif

#include "../Exception.hpp"

namespace page { namespace err { namespace opengl
{
	std::string GetErrorString(GLenum);

	void ResetError()
	{
		for (unsigned i = 0; i < 1000 && glGetError(); ++i);
	}

	void CheckError()
	{
		GLenum error = glGetError();
		if (error)
			THROW((err::Exception<err::ErrModuleTag, err::OpenglPlatformTag>(GetErrorString(error))))
	}

	std::string GetErrorString(GLenum error)
	{
		return
#ifdef HAVE_GLU
			reinterpret_cast<const char *>(gluErrorString(error));
#else
			"error";
#endif
	}
}}}
