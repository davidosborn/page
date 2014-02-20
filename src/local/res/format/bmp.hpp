/**
 * @copyright
 *
 * Copyright (c) 2006-2014 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the same place
 *    and form as other copyright, license, and disclaimer information.
 *
 * 3. Redistributions in binary form must also include an acknowledgement in the
 *    same place and form as other acknowledgements (such as the credits),
 *    similar in substance to the following:
 *
 *       Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#ifndef    page_local_res_format_bmp_hpp
#   define page_local_res_format_bmp_hpp

#	include <cstdint> // int32_t, uint{16,32}_t

namespace page { namespace res
{
	namespace format
	{
		namespace bmp
		{
			const std::uint16_t type = 0x4d42;

#	pragma pack(push, 1)
			struct BITMAPFILEHEADER
			{
				std::uint16_t bfType;
				std::uint32_t bfSize;
				std::uint16_t bfReserved1, bfReserved2;
				std::uint32_t bfOffBits;
			};
			struct BITMAPINFOHEADER
			{
				std::uint32_t bfSize;
				std::int32_t  biWidth, biHeight;
				std::uint16_t biPlanes, biBitCount;
				std::uint32_t biCompression, biSizeImage;
				std::int32_t  biXPelsPerMeter, biYPelsPerMeter;
				std::uint32_t biClrUsed, biClrImportant;
			};
#	pragma pack(pop)

			const char fileHeaderFormat[] = "wdwwd";
			const char infoHeaderFormat[] = "dddwwdddddd";
		}

		using namespace bmp;
	}

	typedef format::BITMAPFILEHEADER BITMAPFILEHEADER;
	typedef format::BITMAPINFOHEADER BITMAPINFOHEADER;
}}

#endif
