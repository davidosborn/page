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

#include <cassert>
#include "../../../err/Exception.hpp"
#include "../../../res/type/Sound.hpp" // Sound::{decoder,frequency}, GetDuration
#include "../../../res/type/sound/AudioDecoder.hpp" // AudioDecoder::Open
#include "../../../res/type/sound/AudioStream.hpp" // AudioStream::{~AudioStream,Read,Seek}
#include "../../../res/type/sound/openal.hpp" // GetFormat
#include "StreamBuffer.hpp"

namespace page
{
	namespace aud
	{
		namespace openal
		{
			const unsigned bufferSize = 65536;

			// construct/destroy
			StreamBuffer::StreamBuffer(ALuint source, const res::Sound &sound, bool loop, float playPosition) :
				source(source), stream(sound.decoder->Open()),
				format(res::openal::GetFormat(sound)),
				frequency(sound.frequency), loop(loop), end(false),
				bufferData(bufferSize)
			{
				alGenBuffers(buffers.size(), &*buffers.begin());
				if (alGetError())
					THROW((err::Exception<err::AudModuleTag, err::OpenalPlatformTag>("failed to create buffer") <<
						boost::errinfo_api_function("alGenBuffers")))
				if (playPosition)
				{
					assert(playPosition <= GetDuration(sound));
					stream->Seek(playPosition * sound.frequency);
				}
				Queue(&*buffers.begin(), buffers.size());
			}
			StreamBuffer::~StreamBuffer()
			{
				alSourceStop(source);
				alSourcei(source, AL_BUFFER, 0);
				alDeleteBuffers(buffers.size(), &*buffers.begin());
			}

			// update
			void StreamBuffer::Update()
			{
				if (end) return;
				// refill processed buffers
				ALint processed = 0;
				alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed);
				if (processed)
				{
					std::vector<ALuint> buffers(processed);
					alSourceUnqueueBuffers(source, processed, &*buffers.begin());
					if (alGetError())
						THROW((err::Exception<err::AudModuleTag, err::OpenalPlatformTag>("failed to unqueue buffer") <<
							boost::errinfo_api_function("alSourceUnqueueBuffers")))
					Queue(&*buffers.begin(), buffers.size());
				}
				// restart starved source
				ALint state = AL_STOPPED;
				alGetSourcei(source, AL_SOURCE_STATE, &state);
				if (state != AL_PLAYING)
				{
					ALint buffers = 0;
					alGetSourcei(source, AL_BUFFERS_QUEUED, &buffers);
					if (buffers) alSourcePlay(source);
				}
			}

			// buffering
			void StreamBuffer::Queue(const ALuint *buffers, unsigned n)
			{
				for (const ALuint *buffer = buffers; buffer != buffers + n; ++buffer)
				{
					if (loop)
					{
						for (BufferData::iterator iter(bufferData.begin());;)
						{
							iter += stream->Read(&*iter, bufferData.end() - iter);
							if (iter == bufferData.end()) break;
							stream->Seek(0);
						}
						alBufferData(*buffer, format, &*bufferData.begin(), bufferData.size(), frequency);
					}
					else
					{
						unsigned size = stream->Read(&*bufferData.begin(), bufferData.size());
						alBufferData(*buffer, format, &*bufferData.begin(), size, frequency);
						if (size != bufferData.size())
						{
							n = buffer - buffers + 1;
							end = true;
							break;
						}
					}
					if (alGetError())
						THROW((err::Exception<err::AudModuleTag, err::OpenalPlatformTag>("failed to initialize buffer") <<
							boost::errinfo_api_function("alBufferData")))
				}
				alSourceQueueBuffers(source, n, buffers);
				if (alGetError())
					THROW((err::Exception<err::AudModuleTag, err::OpenalPlatformTag>("failed to queue buffer") <<
						boost::errinfo_api_function("alSourceQueueBuffers")))
			}
		}
	}
}
