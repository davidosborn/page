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
