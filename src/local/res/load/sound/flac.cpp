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
#include <memory> // {shared,unique}_ptr

#include <FLAC/stream_decoder.h> // FLAC__StreamDecoder

#include "../../adapt/flac.hpp" // Check, Open
#include "../../type/Sound.hpp"
#include "../../type/sound/FlacDecoder.hpp"
#include "../Registry.hpp" // REGISTER_LOADER

namespace page
{
	namespace res
	{
		Sound *LoadFlacSound(const std::shared_ptr<const Pipe> &pipe)
		{
			assert(pipe);
			const auto sd(Open(*pipe, 0));
			if (!sd) return 0;
			const std::unique_ptr<Sound> sound(new Sound);
			sound->channels = FlacDecoder::LimitChannels(FLAC__stream_decoder_get_channels(sd.get()));
			sound->bitDepth = FlacDecoder::LimitBitDepth(FLAC__stream_decoder_get_bits_per_sample(sd.get()));
			sound->frequency = FLAC__stream_decoder_get_sample_rate(sd.get());
			sound->samples = FLAC__stream_decoder_get_total_samples(sd.get());
			sound->decoder.reset(new FlacDecoder(pipe));
			return sound.release();
		}

		LoadFunction GetFlacSoundLoader(const Pipe &pipe)
		{
			return Check(pipe) ? LoadFlacSound : LoadFunction();
		}

		REGISTER_LOADER(Sound, GetFlacSoundLoader,
			"flac,oga,ogg,ogx", "audio/flac,audio/ogg,audio/x-flac", "", true)
	}
}
