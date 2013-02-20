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
