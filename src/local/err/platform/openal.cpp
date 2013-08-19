#include <string>

#include <AL/al.h> // ALenum, alGetError

#include "../Exception.hpp"

namespace page { namespace err { namespace openal
{
	std::string GetErrorString(ALenum);

	void ResetError()
	{
		alGetError();
	}

	void CheckError()
	{
		ALenum error = alGetError();
		if (error != AL_NO_ERROR)
			THROW((err::Exception<err::ErrModuleTag, err::OpenalPlatformTag>(GetErrorString(error))))
	}

	std::string GetErrorString(ALenum error)
	{
		switch (error)
		{
			case AL_NO_ERROR:          return "no error";
			case AL_INVALID_ENUM:      return "invalid enum";
			case AL_INVALID_NAME:      return "invalid name";
			case AL_INVALID_OPERATION: return "invalid operation";
			case AL_INVALID_VALUE:     return "invalid value";
			case AL_OUT_OF_MEMORY:     return "out of memory";
			default:                   return "unknown error";
		}
	}
}}}
