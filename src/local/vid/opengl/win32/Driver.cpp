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
