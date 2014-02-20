/**
 * @copyright
 *
 * Copyright (c) 2006-2014 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the same place
 *    and form as other copyright, license, and disclaimer information.
 *
 * 3. Redistributions in binary form must also include an acknowledgement in the
 *    same place and form as other acknowledgements (such as the credits),
 *    similar in substance to the following:
 *
 *       Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

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
