#include <cassert>

#include <AL/al.h> // ALenum

#include "../../../err/Exception.hpp"
#include "../Sound.hpp" // Sound::{bitDepth,channels}

namespace page
{
	namespace res
	{
		namespace openal
		{
			ALenum GetFormat(const Sound &sound)
			{
				switch (sound.channels)
				{
					case 1:
					switch (sound.bitDepth)
					{
						case 8:  return AL_FORMAT_MONO8;
						case 16: return AL_FORMAT_MONO16;
						default: THROW((err::Exception<err::ResModuleTag, err::OpenalPlatformTag>("unsupported bit depth")))
					}
					break;
					case 2:
					switch (sound.bitDepth)
					{
						case 8:  return AL_FORMAT_STEREO8;
						case 16: return AL_FORMAT_STEREO16;
						default: THROW((err::Exception<err::ResModuleTag, err::OpenalPlatformTag>("unsupported bit depth")))
					}
					break;
					default: THROW((err::Exception<err::ResModuleTag, err::OpenalPlatformTag>("unsupported channel structure")))
				}
				assert(!"logic error");
			}
		}
	}
}
