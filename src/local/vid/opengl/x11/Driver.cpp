#include <X11/Xlib.h>

#include "../../../err/Exception.hpp"
#include "../../../wnd/x11/Window.hpp" // Window::Get{Display,Screen}
#include "Driver.hpp"
#include "ext.hpp" // InitExt

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			namespace x11
			{
				// construct/destroy
				Driver::Driver(wnd::x11::Window &wnd) : opengl::Driver(wnd)
				{
					Display *display = wnd.GetDisplay();
					// select visual
					int attribs[] =
					{
						GLX_RGBA, True,
						GLX_DOUBLEBUFFER, True,
						GLX_RED_SIZE, 8,
						GLX_GREEN_SIZE, 8,
						GLX_BLUE_SIZE, 8,
						GLX_DEPTH_SIZE, 24,
						None
					};
					XVisualInfo *vi = glXChooseVisual(display, wnd.GetScreen(), attribs);
					if (!vi)
						THROW((err::Exception<err::VidModuleTag, err::X11PlatformTag>("failed to choose visual")))
					if (!(context = glXCreateContext(display, vi, 0, True)))
						THROW((err::Exception<err::VidModuleTag, err::X11PlatformTag>("failed to create context")))
					if (!glXMakeCurrent(display, wnd.GetWindow(), context))
						THROW((err::Exception<err::VidModuleTag, err::X11PlatformTag>("failed to make context current")))
					Init();
				}
				Driver::~Driver()
				{
					Deinit();
					glXDestroyContext(GetWindow().GetDisplay(), context);
				}

				// window access
				wnd::x11::Window &Driver::GetWindow()
				{
					return dynamic_cast<wnd::x11::Window &>(opengl::Driver::GetWindow());
				}
				const wnd::x11::Window &Driver::GetWindow() const
				{
					return dynamic_cast<const wnd::x11::Window &>(opengl::Driver::GetWindow());
				}

				// extension initialization
				void Driver::InitAdapterExt()
				{
					const wnd::x11::Window &wnd(GetWindow());
					InitExt(wnd.GetDisplay(), wnd.GetScreen());
				}

				// framebuffer modifiers
				void Driver::Flush()
				{
					const wnd::x11::Window &wnd(GetWindow());
					glXSwapBuffers(wnd.GetDisplay(), wnd.GetWindow());
				}
			}
		}

		// factory function
		Driver *MakeDriver(wnd::Window &wnd)
		{
			return new opengl::x11::Driver(dynamic_cast<wnd::x11::Window &>(wnd));
		}
	}
}
