#include <cassert>
#include <memory> // {shared,unique}_ptr

#include <vorbis/vorbisfile.h> // OggVorbis_File, ov_pcm_total, vorbis_info

#include "../../../err/Exception.hpp"
#include "../../adapt/vorbis.hpp" // Check, Open
#include "../../type/Sound.hpp"
#include "../../type/sound/VorbisDecoder.hpp"
#include "../LoaderRegistry.hpp" // REGISTER_LOADER

namespace page
{
	namespace res
	{
		Sound *LoadVorbisSound(const std::shared_ptr<const Pipe> &pipe)
		{
			assert(pipe);
			const std::unique_ptr<OggVorbis_File> vf(Open(*pipe));
			if (!vf) return 0;
			vorbis_info *vi = ov_info(vf.get(), -1);
			if (!vi)
				THROW((err::Exception<err::ResModuleTag, err::VorbisPlatformTag>()))
			const std::unique_ptr<Sound> sound(new Sound);
			sound->channels = vi->channels;
			sound->bitDepth = 16;
			sound->frequency = vi->rate;
			sound->samples = ov_pcm_total(vf.get(), -1);
			sound->decoder.reset(new VorbisDecoder(pipe));
			return sound.release();
		}

		LoadFunction GetVorbisSoundLoader(const Pipe &pipe)
		{
			return Check(pipe) ? LoadVorbisSound : LoadFunction();
		}

		REGISTER_LOADER(Sound, GetVorbisSoundLoader,
			"oga,ogg,ogx", "audio/ogg,audio/vorbis,audio/x-vorbis", "", true)
	}
}
