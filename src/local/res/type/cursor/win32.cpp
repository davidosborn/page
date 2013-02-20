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

#include <algorithm> // copy

#include <windows.h>

#include "../../../err/Exception.hpp"
#include "../../../math/Vector.hpp"
#include "../Cursor.hpp"
#include "../Image.hpp"

namespace page
{
	namespace res
	{
		namespace win32
		{
			HCURSOR MakeWin32Cursor(const Cursor &cursor, unsigned size)
			{
				// calculate best fitting cursor size
				math::Vector<2> bestCursorSize(cursor.size * size);
				math::Vector<2, int> maxCursorSize(
					GetSystemMetrics(SM_CXCURSOR),
					GetSystemMetrics(SM_CYCURSOR));
				math::Vector<2, unsigned> cursorSize(Any(bestCursorSize > maxCursorSize) ?
					bestCursorSize * Min(maxCursorSize / bestCursorSize) : bestCursorSize);
				// handle zero cursor size
				if (!All(cursorSize)) return NULL;
				// scale image to cursor size
				Image img = Scale(*cursor.image, cursorSize);
				// convert image to compatible format and alignment
				Image::Channels channels;
				channels.reserve(4);
				channels.push_back(Image::Channel(Image::Channel::blue, 8));
				channels.push_back(Image::Channel(Image::Channel::green, 8));
				channels.push_back(Image::Channel(Image::Channel::red, 8));
				channels.push_back(Image::Channel(Image::Channel::alpha, 8));
				if (img.channels != channels) img = Convert(img, channels, 1);
				else if (!IsAligned(img, 1)) img = Align(img, 1);
				// create color and mask bitmaps
				// on Windows XP or greater, use alpha-blended DIB cursors
				// otherwise, use bitwise-masked DDB cursors
				HBITMAP colorBitmap, maskBitmap;
				OSVERSIONINFO vi = {sizeof vi};
				if (!GetVersionEx(&vi))
					THROW((err::Exception<err::ResModuleTag, err::Win32PlatformTag>("failed to get Windows version")))
				if (vi.dwPlatformId == VER_PLATFORM_WIN32_NT &&
					vi.dwMajorVersion >= 5 && vi.dwMinorVersion >= 1)
				{
					// create color bitmap
					BITMAPV5HEADER bmv5h =
					{
						sizeof bmv5h,
						cursorSize.x,
						-cursorSize.y,
						1, 32, BI_BITFIELDS,
						0, 0, 0, 0, 0,
						0x00ff0000, // red
						0x0000ff00, // green
						0x000000ff, // blue
						0xff000000  // alpha
					};
					LPVOID colorBits;
					HDC hdc = GetDC(NULL);
					colorBitmap = CreateDIBSection(hdc, reinterpret_cast<LPBITMAPINFO>(&bmv5h), DIB_RGB_COLORS, &colorBits, NULL, 0);
					ReleaseDC(NULL, hdc);
					if (!colorBitmap)
						THROW((err::Exception<err::ResModuleTag, err::Win32PlatformTag>("failed to create bitmap")))
					// copy image data to color bitmap
					std::copy(img.data.begin(), img.data.end(),
						reinterpret_cast<Image::Data::pointer>(colorBits));
					// create mask bitmap
					maskBitmap = CreateBitmap(cursorSize.x, cursorSize.y, 1, 1, 0);
					if (!maskBitmap)
					{
						DeleteObject(colorBitmap);
						THROW((err::Exception<err::ResModuleTag, err::Win32PlatformTag>("failed to create bitmap")))
					}
				}
				else
				{
					// create color bitmap
					BITMAPINFOHEADER bmih =
					{
						sizeof bmih,
						cursorSize.x,
						-int(cursorSize.y),
						1, 32, BI_RGB
					};
					HDC hdc = GetDC(NULL);
					if (!(colorBitmap = CreateCompatibleBitmap(hdc, cursorSize.x, cursorSize.y)))
					{
						ReleaseDC(NULL, hdc);
						THROW((err::Exception<err::ResModuleTag, err::Win32PlatformTag>("failed to create bitmap")))
					}
					// set color and alpha for masking
					for (Image::Data::iterator iter(img.data.begin()); iter != img.data.end(); iter += img.channels.size())
						if (iter[3] >= 0x80) iter[3] = 0xff;
						else iter[0] = iter[1] = iter[2] = iter[3] = 0;
					// copy image data to color bitmap
					if (!SetDIBits(hdc, colorBitmap, 0, img.size.y, &*img.data.begin(), reinterpret_cast<LPBITMAPINFO>(&bmih), DIB_RGB_COLORS))
					{
						DeleteObject(colorBitmap);
						ReleaseDC(NULL, hdc);
						THROW((err::Exception<err::ResModuleTag, err::Win32PlatformTag>("failed to initialize bitmap")))
					}
					// create mask bitmap
					if (!(maskBitmap = CreateCompatibleBitmap(hdc, cursorSize.x, cursorSize.y)))
					{
						DeleteObject(colorBitmap);
						ReleaseDC(NULL, hdc);
						THROW((err::Exception<err::ResModuleTag, err::Win32PlatformTag>("failed to create bitmap")))
					}
					// replace color with alpha
					for (Image::Data::iterator iter(img.data.begin()); iter != img.data.end(); iter += img.channels.size())
						iter[0] = iter[1] = iter[2] = iter[3];
					// copy image data to mask bitmap
					if (!SetDIBits(hdc, maskBitmap, 0, img.size.y, &*img.data.begin(), reinterpret_cast<LPBITMAPINFO>(&bmih), DIB_RGB_COLORS))
					{
						DeleteObject(maskBitmap);
						DeleteObject(colorBitmap);
						ReleaseDC(NULL, hdc);
						THROW((err::Exception<err::ResModuleTag, err::Win32PlatformTag>("failed to initialize bitmap")))
					}
					ReleaseDC(NULL, hdc);
				}
				// create cursor
				math::Vector<2, DWORD> hotspot(cursor.center * cursorSize);
				ICONINFO ii =
				{
					FALSE,
					hotspot.x, hotspot.y,
					maskBitmap, colorBitmap
				};
				HCURSOR win32Cursor = CreateIconIndirect(&ii);
				DeleteObject(maskBitmap);
				DeleteObject(colorBitmap);
				if (!win32Cursor)
					THROW((err::Exception<err::ResModuleTag, err::Win32PlatformTag>("failed to create cursor")))
				return win32Cursor;
			}
		}
	}
}
