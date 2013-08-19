#include <cassert>
#include <cstdint> // uint16_t
#include <cstring> // memcmp
#include <memory> // {shared,unique}_ptr

#include "../../../err/Exception.hpp"
#include "../../../util/endian.hpp" // littleEndian, TransformEndian
#include "../../fmt/wav.hpp"
#include "../../pipe/Pipe.hpp" // Pipe::Open
#include "../../pipe/SubPipe.hpp"
#include "../../pipe/Stream.hpp"
#include "../../type/Sound.hpp"
#include "../../type/sound/PcmDecoder.hpp"
#include "../LoaderRegistry.hpp" // REGISTER_LOADER

namespace page
{
	namespace res
	{
		Sound *LoadWavSound(const std::shared_ptr<const Pipe> &pipe)
		{
			assert(pipe);
			const std::unique_ptr<Stream> stream(pipe->Open());
			fmt::Header header;
			if (stream->ReadSome(&header, sizeof header) != sizeof header ||
				std::memcmp(header.id, fmt::riff, sizeof header.id) ||
				std::memcmp(header.format, fmt::wave, sizeof header.format)) return 0;
			fmt::Format format;
			stream->Read(&format, sizeof format);
			util::TransformEndian(&format, fmt::formatFormat, util::littleEndian);
			if (std::memcmp(format.id, fmt::fmt, sizeof format.id))
				THROW((err::Exception<err::ResModuleTag, err::FormatTag>("unexpected chunk id")))
			if (format.format != 1)
				THROW((err::Exception<err::ResModuleTag, err::FormatTag>("unsupported compression format")))
			switch (format.size)
			{
				case 16:
				case 18:
				case 40: break;
				default: THROW((err::Exception<err::ResModuleTag, err::FormatTag>("invalid format chunk size")))
			}
			if (!format.channels)
				THROW((err::Exception<err::ResModuleTag, err::FormatTag>("no channels")))
			if (format.channels > 2)
				THROW((err::Exception<err::ResModuleTag, err::FormatTag>("too many channels")))
			if (format.byteRate != format.frequency * format.blockAlign)
				THROW((err::Exception<err::ResModuleTag, err::FormatTag>("invalid byte rate")))
			if (format.blockAlign != format.channels * format.bitDepth / 8)
				THROW((err::Exception<err::ResModuleTag, err::FormatTag>("invalid block alignment")))
			switch (format.bitDepth)
			{
				case 8:
				case 16:
				case 24:
				case 32: break;
				default: THROW((err::Exception<err::ResModuleTag, err::FormatTag>("invalid bit depth")))
			}
			if (format.size >= 18)
			{
				std::uint16_t extSize;
				stream->Read(&extSize, sizeof extSize);
				util::TransformEndian(&extSize, "w", util::littleEndian);
				if (extSize && format.size != 18 + extSize)
					THROW((err::Exception<err::ResModuleTag, err::FormatTag>("invalid extension size")))
				if (extSize != 22)
					THROW((err::Exception<err::ResModuleTag, err::FormatTag>("unsupported extension")))
				fmt::ExtFormat format;
				stream->Read(&format, sizeof format);
				util::TransformEndian(&format, fmt::extFormatFormat, util::littleEndian);
				if (std::memcmp(format.guid, fmt::pcm, sizeof format.guid))
					THROW((err::Exception<err::ResModuleTag, err::FormatTag>("unsupported extension")))
			}
			fmt::Data data;
			stream->Read(&data, sizeof data);
			util::TransformEndian(&data, fmt::dataFormat, util::littleEndian);
			if (std::memcmp(data.id, fmt::data, sizeof data.id))
				THROW((err::Exception<err::ResModuleTag, err::FormatTag>("unexpected chunk id")))
			if (data.size % format.blockAlign)
				THROW((err::Exception<err::ResModuleTag, err::FormatTag>("invalid data size")))
			const std::unique_ptr<Sound> sound(new Sound);
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
			const std::unique_ptr<Stream> stream(pipe.Open());
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
