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

#include <memory> // unique_ptr
#include <vector>

#include "../../../err/Exception.hpp"
#include "../../../res/type/Sound.hpp" // GetSize, Sound::{decoder,frequency}
#include "../../../res/type/sound/AudioDecoder.hpp" // AudioDecoder::Open
#include "../../../res/type/sound/AudioStream.hpp" // AudioStream::{~AudioStream,Read}
#include "../../../res/type/sound/openal.hpp" // GetFormat
#include "BufferProxy.hpp"

namespace page { namespace cache { namespace openal
{
	namespace
	{
		/**
		 *
		 */
		struct BufferDeleter
		{
			typedef void result_type;
			typedef const ALuint *argument_type;

			void operator ()(const ALuint *buffer) const
			{
				alDeleteBuffers(1, buffer);
				delete buffer;
			}
		};
	}

	/*-------------+
	| constructors |
	+-------------*/

	BufferProxy::BufferProxy(const Proxy<res::Sound> &sound) :
		BasicProxy<ALuint>(Signature("OpenAL audio buffer", sound)),
		sound(sound) {}

	/*--------------------------+
	| BasicProxy implementation |
	+--------------------------*/

	auto BufferProxy::DoLock() const -> pointer
	{
		ALuint buffer;
		alGenBuffers(1, &buffer);
		if (alGetError())
			THROW((err::Exception<err::CacheModuleTag, err::OpenalPlatformTag>("failed to create buffer") <<
				boost::errinfo_api_function("alGenBuffers")))
		try
		{
			const res::Sound &sound(*this->sound);
			const std::unique_ptr<res::AudioStream> stream(sound.decoder->Open());
			std::vector<char> data(GetSize(sound));
			data.resize(stream->Read(&*data.begin(), data.size()));
			alBufferData(buffer, res::openal::GetFormat(sound),
				&*data.begin(), data.size(), sound.frequency);
			if (alGetError())
				THROW((err::Exception<err::CacheModuleTag, err::OpenalPlatformTag>("failed to initialize buffer") <<
					boost::errinfo_api_function("alBufferData")))
			return pointer(new ALuint(buffer), BufferDeleter());
		}
		catch (...)
		{
			alDeleteBuffers(1, &buffer);
		}
	}
}}}
