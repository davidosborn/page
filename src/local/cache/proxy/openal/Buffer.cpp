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

#include <vector>
#include "../../../err/exception/throw.hpp" // THROW
#include "../../../res/type/Sound.hpp" // GetSize, Sound::{decoder,frequency}
#include "../../../res/type/sound/AudioDecoder.hpp" // AudioDecoder::Open
#include "../../../res/type/sound/AudioStream.hpp" // AudioStream::{~AudioStream,Read}
#include "../../../res/type/sound/openal.hpp" // GetFormat
#include "../../../util/scoped_ptr.hpp"
#include "Buffer.hpp"

namespace page
{
	namespace res { class Sound; }

	namespace cache
	{
		namespace openal
		{
			namespace
			{
				void Deleter(const ALuint *buffer)
				{
					alDeleteBuffers(1, buffer);
					delete buffer;
				}
			}

			// construct
			Buffer::Buffer(const Proxy<res::Sound> &sound) :
				sound(sound.Copy()) {}

			// clone
			Buffer *Buffer::Clone() const
			{
				return new Buffer(*this);
			}

			// attributes
			std::string Buffer::GetType() const
			{
				return "audio buffer";
			}
			std::string Buffer::GetSource() const
			{
				return sound->GetSource();
			}

			// dependency satisfaction
			Buffer::operator bool() const
			{
				return *sound;
			}

			// instantiation
			Buffer::Instance Buffer::Make() const
			{
				ALuint buffer;
				alGenBuffers(1, &buffer);
				if (alGetError()) THROW err::PlatformException<err::OpenalPlatformTag>("failed to create buffer");
				try
				{
					const res::Sound &sound(**this->sound);
					util::scoped_ptr<res::AudioStream> stream(sound.decoder->Open());
					std::vector<char> data(GetSize(sound));
					data.resize(stream->Read(&*data.begin(), data.size()));
					alBufferData(buffer, res::openal::GetFormat(sound),
						&*data.begin(), data.size(), sound.frequency);
					if (alGetError()) THROW err::PlatformException<err::OpenalPlatformTag>("failed to initialize buffer");
					return Instance(new ALuint(buffer), Deleter);
				}
				catch (...)
				{
					alDeleteBuffers(1, &buffer);
				}
			}
		}
	}
}
