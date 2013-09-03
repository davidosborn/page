#include <algorithm> // min
#include <cassert>
#include <cstring> // memcpy
#include <functional> // bind

#include "../../../err/Exception.hpp"
#include "../../adapt/flac.hpp" // Open
#include "FlacStream.hpp"

namespace page
{
	namespace res
	{
		/*-------------+
		| constructors |
		+-------------*/

		FlacStream::FlacStream(const Pipe &pipe) : init(false),
			sd(Open(pipe, std::bind(&FlacStream::Write, this,
				std::placeholders::_1,
				std::placeholders::_2,
				std::placeholders::_3)))
		{
			if (!sd)
				THROW((err::Exception<err::ResModuleTag, err::FlacPlatformTag, err::FormatTag>()))
		}

		/*-----------+
		| operations |
		+-----------*/

		unsigned FlacStream::Read(void *s, unsigned n)
		{
			unsigned i = 0;
			for (;;)
			{
				unsigned buffered = std::min(n - i, buffer.size());
				std::memcpy(s, &*buffer.begin(), buffered);
				buffer.erase(buffer.begin(), buffer.begin() + buffered);
				s = static_cast<char *>(s) + buffered;
				if ((i += buffered) == n) break;
				if (!FLAC__stream_decoder_process_single(sd.get()))
					CheckError(sd.get());
				if (buffer.empty()) break;
			}
			return i;
		}
		void FlacStream::Seek(unsigned sample)
		{
			if (!FLAC__stream_decoder_seek_absolute(sd.get(), sample))
				CheckError(sd.get());
			buffer.clear();
		}

		/*---------------+
		| implementation |
		+---------------*/

		void FlacStream::Write(const FLAC__StreamDecoder *sd, const FLAC__Frame *frame, const FLAC__int32 *const buffers[])
		{
			assert(
				frame->header.blocksize &&
				frame->header.channels &&
				frame->header.sample_rate &&
				frame->header.bits_per_sample);
			if (!init)
			{
				channels = FlacDecoder::LimitChannels(FLAC__stream_decoder_get_channels(sd));
				bitDepth = FlacDecoder::LimitBitDepth(FLAC__stream_decoder_get_bits_per_sample(sd));
				init = true;
			}
			unsigned delta = frame->header.blocksize * channels * bitDepth / 8;
			buffer.reserve(buffer.size() + delta);
			if (channels == 1)
			{
				const FLAC__int32 *const samples = buffers[0];
				switch (bitDepth)
				{
					case 8:
					for (unsigned i = 0; i < frame->header.blocksize; ++i)
					{
						buffer.push_back(samples[i]);
					}
					break;
					case 16:
					for (unsigned i = 0; i < frame->header.blocksize; ++i)
					{
#ifdef WORDS_BIGENDIAN
						buffer.push_back(samples[i] >> 8);
						buffer.push_back(samples[i]);
#else
						buffer.push_back(samples[i]);
						buffer.push_back(samples[i] >> 8);
#endif
					}
					break;
					case 24:
					for (unsigned i = 0; i < frame->header.blocksize; ++i)
					{
#ifdef WORDS_BIGENDIAN
						buffer.push_back(samples[i] >> 16);
						buffer.push_back(samples[i] >> 8);
						buffer.push_back(samples[i]);
#else
						buffer.push_back(samples[i]);
						buffer.push_back(samples[i] >> 8);
						buffer.push_back(samples[i] >> 16);
#endif
					}
					break;
					case 32:
					for (unsigned i = 0; i < frame->header.blocksize; ++i)
					{
#ifdef WORDS_BIGENDIAN
						buffer.push_back(samples[i] >> 24);
						buffer.push_back(samples[i] >> 16);
						buffer.push_back(samples[i] >> 8);
						buffer.push_back(samples[i]);
#else
						buffer.push_back(samples[i]);
						buffer.push_back(samples[i] >> 8);
						buffer.push_back(samples[i] >> 16);
						buffer.push_back(samples[i] >> 24);
#endif
					}
					break;
				}
			}
			else
			{
				assert(channels == 2);
				const FLAC__int32 *const samples[2] =
				{
					buffers[0],
					frame->header.channels >= 2 ? buffers[1] : buffers[0]
				};
				switch (bitDepth)
				{
					case 8:
					for (unsigned i = 0; i < frame->header.blocksize; ++i)
					{
						buffer.push_back(samples[0][i]);
						buffer.push_back(samples[1][i]);
					}
					break;
					case 16:
					for (unsigned i = 0; i < frame->header.blocksize; ++i)
					{
#ifdef WORDS_BIGENDIAN
						buffer.push_back(samples[0][i] >> 8);
						buffer.push_back(samples[0][i]);
						buffer.push_back(samples[1][i] >> 8);
						buffer.push_back(samples[1][i]);
#else
						buffer.push_back(samples[0][i]);
						buffer.push_back(samples[0][i] >> 8);
						buffer.push_back(samples[1][i]);
						buffer.push_back(samples[1][i] >> 8);
#endif
					}
					break;
					case 24:
					for (unsigned i = 0; i < frame->header.blocksize; ++i)
					{
#ifdef WORDS_BIGENDIAN
						buffer.push_back(samples[0][i] >> 16);
						buffer.push_back(samples[0][i] >> 8);
						buffer.push_back(samples[0][i]);
						buffer.push_back(samples[1][i] >> 16);
						buffer.push_back(samples[1][i] >> 8);
						buffer.push_back(samples[1][i]);
#else
						buffer.push_back(samples[0][i]);
						buffer.push_back(samples[0][i] >> 8);
						buffer.push_back(samples[0][i] >> 16);
						buffer.push_back(samples[1][i]);
						buffer.push_back(samples[1][i] >> 8);
						buffer.push_back(samples[1][i] >> 16);
#endif
					}
					break;
					case 32:
					for (unsigned i = 0; i < frame->header.blocksize; ++i)
					{
#ifdef WORDS_BIGENDIAN
						buffer.push_back(samples[0][i] >> 24);
						buffer.push_back(samples[0][i] >> 16);
						buffer.push_back(samples[0][i] >> 8);
						buffer.push_back(samples[0][i]);
						buffer.push_back(samples[1][i] >> 24);
						buffer.push_back(samples[1][i] >> 16);
						buffer.push_back(samples[1][i] >> 8);
						buffer.push_back(samples[1][i]);
#else
						buffer.push_back(samples[0][i]);
						buffer.push_back(samples[0][i] >> 8);
						buffer.push_back(samples[0][i] >> 16);
						buffer.push_back(samples[0][i] >> 24);
						buffer.push_back(samples[1][i]);
						buffer.push_back(samples[1][i] >> 8);
						buffer.push_back(samples[1][i] >> 16);
						buffer.push_back(samples[1][i] >> 24);
#endif
					}
					break;
				}
			}
		}
	}
}
