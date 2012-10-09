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

#include <windows.h>
#include "../../../err/exception/throw.hpp" // THROW
#include "../Image.hpp"

namespace page
{
	namespace res
	{
		namespace win32
		{
			HBITMAP MakeBitmap(const Image &img)
			{
				// convert image to compatible format and alignment
				Image::Channels channels;
				channels.reserve(3);
				channels.push_back(Image::Channel(Image::Channel::blue,  8));
				channels.push_back(Image::Channel(Image::Channel::green, 8));
				channels.push_back(Image::Channel(Image::Channel::red,   8));
				Image scratchImg;
				const Image *compatImg = &img;
				if (img.channels != channels)
				{
					scratchImg = Convert(*compatImg, channels, 4);
					compatImg = &scratchImg;
				}
				else if (!IsAligned(img, 4))
				{
					scratchImg = Align(*compatImg, 4);
					compatImg = &scratchImg;
				}
				// create bitmap
				BITMAPINFOHEADER bmih =
				{
					sizeof bmih,
					compatImg->size.x,
					-int(compatImg->size.y),
					1, 24, BI_RGB
				};
				HDC hdc = GetDC(NULL);
				HBITMAP bm = CreateCompatibleBitmap(hdc, compatImg->size.x, compatImg->size.y);
				if (!bm)
				{
					ReleaseDC(NULL, hdc);
					THROW err::PlatformException<err::Win32PlatformTag, err::ResourceTag>("failed to create bitmap");
				}
				// copy image data to bitmap
				if (!SetDIBits(hdc, bm, 0, compatImg->size.y, &*compatImg->data.begin(), reinterpret_cast<LPBITMAPINFO>(&bmih), DIB_RGB_COLORS))
				{
					DeleteObject(bm);
					ReleaseDC(NULL, hdc);
					THROW err::PlatformException<err::Win32PlatformTag, err::ResourceTag>("failed to initialize bitmap");
				}
				ReleaseDC(NULL, hdc);
				return bm;
			}
		}
	}
}
