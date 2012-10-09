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

#include <cassert>
#include "../../../cache/Proxy.hpp"
#include "../../../cache/proxy/openal/Buffer.hpp"
#include "../../../res/type/Sound.hpp" // GetDuration
#include "StaticBuffer.hpp"

namespace page
{
	namespace aud
	{
		namespace openal
		{
			// construct/destroy
			StaticBuffer::StaticBuffer(ALuint source, const cache::Proxy<res::Sound> &sound, bool loop, float playPosition) :
				source(source), buffer(cache::openal::Buffer(sound).Copy()),
				lock(buffer->lock())
			{
				alSourcei(source, AL_BUFFER, *lock);
				alSourcei(source, AL_LOOPING, loop);
				// FIXME: consider using an extension mechanism similar to that
				// of the OpenGL video module
				if (playPosition && alIsExtensionPresent("AL_EXT_OFFSET"))
				{
					assert(playPosition <= GetDuration(*sound));
					alSourcef(source, AL_SEC_OFFSET, playPosition);
				}
			}
			StaticBuffer::~StaticBuffer()
			{
				alSourceStop(source);
				alSourcei(source, AL_BUFFER, 0);
			}

			// update
			void StaticBuffer::Update() {}
		}
	}
}
