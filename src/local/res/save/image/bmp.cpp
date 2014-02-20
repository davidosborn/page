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

#include <climits> // CHAR_BIT
#include <ostream>

#include "../../../err/Exception.hpp"
#include "../../../util/endian.hpp" // TransformEndian
#include "../../fmt/bmp.hpp"
#include "../../type/Image.hpp"
#include "../register.hpp" // REGISTER_SAVER

namespace page { namespace res
{
	namespace
	{
		inline void Write(std::ostream &os, const void *s, unsigned n)
		{
			if (!os.write(static_cast<const char *>(s), n))
				THROW((err::Exception<err::ResModuleTag, err::FileWriteTag>()))
		}
	}

	void SaveBmpImage(const Image &img, std::ostream &os)
	{
		// convert image to compatible format
		// FIXME: use closest compatible format
		Image scratchImg;
		const Image *compatImg = &img;
		Image::Channels channels;
		channels.reserve(3);
		channels.push_back(Image::Channel(Image::Channel::blue, CHAR_BIT));
		channels.push_back(Image::Channel(Image::Channel::green, CHAR_BIT));
		channels.push_back(Image::Channel(Image::Channel::red, CHAR_BIT));
		if (img.channels != channels)
		{
			scratchImg = Convert(*compatImg, channels, 4);
			compatImg = &scratchImg;
		}
		else if (!IsAligned(*compatImg, 4))
		{
			scratchImg = Align(*compatImg, 4);
			compatImg = &scratchImg;
		}
		// write bmp file
		BITMAPFILEHEADER bmfh =
		{
			fmt::type,
			sizeof bmfh + sizeof(BITMAPINFOHEADER) + compatImg->data.size(), 0, 0,
			sizeof bmfh + sizeof(BITMAPINFOHEADER)
		};
		util::TransformEndian(&bmfh, fmt::fileHeaderFormat, util::nativeEndian, util::littleEndian);
		Write(os, &bmfh, sizeof bmfh);
		BITMAPINFOHEADER bmih =
		{
			sizeof bmih,
			compatImg->size.x, -compatImg->size.y,
			1, static_cast<std::uint16_t>(GetDepth(compatImg->channels))
		};
		util::TransformEndian(&bmih, fmt::infoHeaderFormat, util::nativeEndian, util::littleEndian);
		Write(os, &bmih, sizeof bmih);
		Write(os, &*compatImg->data.begin(), compatImg->data.size());
	}

#if HOST_OS == win32
#	define SAVER_PRIORITY 80
#else
#	define SAVER_PRIORITY 20
#endif

	REGISTER_SAVER(Image, SaveBmpImage, "bmp", "bmp", SAVER_RANK)
}}
