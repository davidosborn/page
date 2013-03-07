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

#include <vorbis/vorbisfile.h> // OggVorbis_File, ov_pcm_total, vorbis_info

#include "../../../err/Exception.hpp"
#include "../../adapt/vorbis.hpp" // Check, Open
#include "../../type/Sound.hpp"
#include "../../type/sound/VorbisDecoder.hpp"
#include "../Registry.hpp" // REGISTER_LOADER

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
