/**
 * @section license
 *
 * Copyright (c) 2006-2012 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution, and in the same
 *    place and form as other copyright, license, and disclaimer information.
 *
 * As a special exception, distributions of derivative works in binary form may
 * include an acknowledgement in place of the above copyright notice, this list
 * of conditions, and the following disclaimer in the documentation and/or other
 * materials provided with the distribution, and in the same place and form as
 * other acknowledgements, similar in substance to the following:
 *
 *    Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#include <string>

// OpenAL
#include <AL/al.h> // ALenum, alGetError

// local
#include "Exception.hpp"

namespace page
{
	namespace err
	{
		namespace openal
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
		}
	}
}
