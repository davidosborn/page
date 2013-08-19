#include <cassert>
#include <memory> // {shared,unique}_ptr

#include <FLAC/stream_decoder.h> // FLAC__StreamDecoder

#include "../../adapt/flac.hpp" // Check, Open
#include "../../type/Sound.hpp"
#include "../../type/sound/FlacDecoder.hpp"
#include "../LoaderRegistry.hpp" // REGISTER_LOADER

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
