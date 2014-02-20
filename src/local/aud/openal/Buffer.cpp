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

#include "../../cache/proxy/Proxy.hpp"
#include "../../res/type/Sound.hpp" // GetSize
#include "Buffer.hpp"
#include "buffer/StaticBuffer.hpp"
#include "buffer/StreamBuffer.hpp"

namespace page { namespace aud { namespace openal
{
	// maximum size of audio data before streaming kicks in
	const unsigned maxStaticSize = 262144;

	// destroy
	Buffer::~Buffer() {}

	// factory function
	Buffer *MakeBuffer(ALuint source, const cache::Proxy<res::Sound> &sound, bool loop, float playPosition)
	{
		return GetSize(*sound) <= maxStaticSize ?
			static_cast<Buffer *>(new StaticBuffer(source, sound, loop, playPosition)) :
			static_cast<Buffer *>(new StreamBuffer(source, *sound, loop, playPosition));
	}
}}}
