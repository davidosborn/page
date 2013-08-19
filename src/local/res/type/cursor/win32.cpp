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
				math::Vec2 bestCursorSize(cursor.size * size);
				math::Vec2i maxCursorSize(
					GetSystemMetrics(SM_CXCURSOR),
					GetSystemMetrics(SM_CYCURSOR));
				math::Vec2u cursorSize(Any(bestCursorSize > maxCursorSize) ?
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
