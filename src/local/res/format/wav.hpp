#ifndef    page_local_res_format_wav_hpp
#   define page_local_res_format_wav_hpp

#	include <cstdint> // uint{16,32}_t

namespace page { namespace res { namespace format
{
	namespace wav
	{
		const char
			riff[] = {'R', 'I', 'F', 'F'},
			wave[] = {'W', 'A', 'V', 'E'},
			fmt[]  = {'f', 'm', 't', ' '},
			data[] = {'d', 'a', 't', 'a'};

		const unsigned char pcm[] =
		{
			0x01, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x10, 0x00,
			0x80, 0x00, 0x00, 0xaa,
			0x00, 0x38, 0x9b, 0x71
		};

#	pragma pack(push, 1)
		struct Header
		{
			char id[4];
			std::uint32_t size;
			char format[4];
		};
		struct Format
		{
			char id[4];
			std::uint32_t size;
			std::uint16_t format;
			std::uint16_t channels;
			std::uint32_t frequency;
			std::uint32_t byteRate;
			std::uint16_t blockAlign;
			std::uint16_t bitDepth;
		};
		struct ExtFormat
		{
			std::uint16_t validBits;
			std::uint32_t speakerMask;
			unsigned char guid[16];
		};
		struct Data
		{
			char id[4];
			std::uint32_t size;
		};
#	pragma pack(pop)

		const char headerFormat[]    = "bbbbdbbbb";
		const char formatFormat[]    = "bbbbdwwddww";
		const char extFormatFormat[] = "wdbbbbbbbbbbbbbbbb";
		const char dataFormat[]      = "bbbbd";
	}

	using namespace wav;
}}}

#endif
