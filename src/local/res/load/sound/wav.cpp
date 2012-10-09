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
#include <cstdint> // uint16_t
#include <cstring> // memcmp
#include <memory> // shared_ptr
#include "../../../err/exception/throw.hpp" // THROW
#include "../../../util/endian.hpp" // littleEndian, TransformEndian
#include "../../../util/scoped_ptr.hpp"
#include "../../fmt/wav.hpp"
#include "../../Pipe.hpp" // Pipe::Open
#include "../../pipe/SubPipe.hpp"
#include "../../Stream.hpp"
#include "../../type/Sound.hpp"
#include "../../type/sound/PcmDecoder.hpp"
#include "../register.hpp" // LoadFunction, REGISTER_LOADER

namespace page
{
	namespace res
	{
		Sound *LoadWavSound(const std::shared_ptr<const Pipe> &pipe)
		{
			assert(pipe);
			util::scoped_ptr<Stream> stream(pipe->Open());
			fmt::Header header;
			if (stream->ReadSome(&header, sizeof header) != sizeof header ||
				std::memcmp(header.id, fmt::riff, sizeof header.id) ||
				std::memcmp(header.format, fmt::wave, sizeof header.format)) return 0;
			fmt::Format format;
			stream->Read(&format, sizeof format);
			util::TransformEndian(&format, fmt::formatFormat, util::littleEndian);
			if (std::memcmp(format.id, fmt::fmt, sizeof format.id))
				THROW err::FormatException<err::ResourceTag>("unexpected chunk id");
			if (format.format != 1)
				THROW err::FormatException<err::ResourceTag>("unsupported compression format");
			switch (format.size)
			{
				case 16:
				case 18:
				case 40: break;
				default: THROW err::FormatException<err::ResourceTag>("invalid format chunk size");
			}
			if (!format.channels)
				THROW err::FormatException<err::ResourceTag>("no channels");
			if (format.channels > 2)
				THROW err::FormatException<err::ResourceTag>("too many channels");
			if (format.byteRate != format.frequency * format.blockAlign)
				THROW err::FormatException<err::ResourceTag>("invalid byte rate");
			if (format.blockAlign != format.channels * format.bitDepth / 8)
				THROW err::FormatException<err::ResourceTag>("invalid block alignment");
			switch (format.bitDepth)
			{
				case 8:
				case 16:
				case 24:
				case 32: break;
				default: THROW err::FormatException<err::ResourceTag>("invalid bit depth");
			}
			if (format.size >= 18)
			{
				std::uint16_t extSize;
				stream->Read(&extSize, sizeof extSize);
				util::TransformEndian(&extSize, "w", util::littleEndian);
				if (extSize && format.size != 18 + extSize)
					THROW err::FormatException<err::ResourceTag>("invalid extension size");
				if (extSize != 22)
					THROW err::FormatException<err::ResourceTag>("unsupported extension");
				fmt::ExtFormat format;
				stream->Read(&format, sizeof format);
				util::TransformEndian(&format, fmt::extFormatFormat, util::littleEndian);
				if (std::memcmp(format.guid, fmt::pcm, sizeof format.guid))
					THROW err::FormatException<err::ResourceTag>("unsupported extension");
			}
			fmt::Data data;
			stream->Read(&data, sizeof data);
			util::TransformEndian(&data, fmt::dataFormat, util::littleEndian);
			if (std::memcmp(data.id, fmt::data, sizeof data.id))
				THROW err::FormatException<err::ResourceTag>("unexpected chunk id");
			if (data.size % format.blockAlign)
				THROW err::FormatException<err::ResourceTag>("invalid data size");
			util::scoped_ptr<Sound> sound(new Sound);
			sound->channels = format.channels;
			sound->bitDepth = format.bitDepth;
			sound->frequency = format.frequency;
			sound->samples = data.size / format.blockAlign;
			sound->decoder.reset(new PcmDecoder(
				std::shared_ptr<Pipe>(new SubPipe(pipe, stream->Tell(), data.size)),
				format.bitDepth, util::littleEndian));
			return sound.release();
		}

		LoadFunction GetWavSoundLoader(const Pipe &pipe)
		{
			util::scoped_ptr<Stream> stream(pipe.Open());
			fmt::Header header;
			return stream->ReadSome(&header, sizeof header) == sizeof header &&
				!std::memcmp(header.id, fmt::riff, sizeof header.id) &&
				!std::memcmp(header.format, fmt::wave, sizeof header.format) ?
				LoadWavSound : LoadFunction();
		}

		REGISTER_LOADER(Sound, GetWavSoundLoader,
			"wav", "audio/wav,audio/wave,audio/x-wav", "", true)
	}
}
