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

#include <windows.h>

#include "../../../err/Exception.hpp"
#include "../../../wnd/win32/Window.hpp" // Window::GetHwnd
#include "Driver.hpp"
#include "ext.hpp" // InitExt

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			namespace win32
			{
				// construct/destroy
				Driver::Driver(wnd::win32::Window &wnd) :
					opengl::Driver(wnd), hwnd(wnd.GetHwnd())
				{
					if (!(hdc = GetDC(hwnd)))
						THROW((err::Exception<err::VidModuleTag, err::Win32PlatformTag>("failed to get device context")))
					PIXELFORMATDESCRIPTOR pfd = {sizeof pfd, 1};
					pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
					pfd.iPixelType = PFD_TYPE_RGBA;
					pfd.cColorBits = 24;
					pfd.cDepthBits = 24;
					int pf = ChoosePixelFormat(hdc, &pfd);
					if (!pf)
					{
						ReleaseDC(hwnd, hdc);
						THROW((err::Exception<err::VidModuleTag, err::Win32PlatformTag>("failed to choose pixel format")))
					}
					if (!SetPixelFormat(hdc, pf, &pfd))
					{
						ReleaseDC(hwnd, hdc);
						THROW((err::Exception<err::VidModuleTag, err::Win32PlatformTag>("failed to set pixel format")))
					}
					if (!(hglrc = wglCreateContext(hdc)))
					{
						ReleaseDC(hwnd, hdc);
						THROW((err::Exception<err::VidModuleTag, err::Win32PlatformTag>("failed to create context")))
					}
					if (!wglMakeCurrent(hdc, hglrc))
					{
						wglDeleteContext(hglrc);
						ReleaseDC(hwnd, hdc);
						THROW((err::Exception<err::VidModuleTag, err::Win32PlatformTag>("failed to make context current")))
					}
					Init();
				}
				Driver::~Driver()
				{
					Deinit();
					wglMakeCurrent(0, 0);
					wglDeleteContext(hglrc);
					ReleaseDC(hwnd, hdc);
				}

				// extension initialization
				void Driver::InitAdapterExt()
				{
					InitExt(hdc);
				}

				// framebuffer modifiers
				void Driver::Flush()
				{
					SwapBuffers(hdc);
				}
			}
		}

		// factory function
		Driver *MakeDriver(wnd::Window &wnd)
		{
			return new opengl::win32::Driver(dynamic_cast<wnd::win32::Window &>(wnd));
		}
	}
}
