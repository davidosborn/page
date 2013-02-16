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

#include <array>

// X11
#include <X11/Xatom.h> // XA_STRING
#include <X11/Xutil.h> // XSetWMName, XTextProperty

// local
#include "../../aud/Driver.hpp" // MakeDriver
#include "../../cfg/vars.hpp"
#include "../../err/Exception.hpp"
#include "../../inp/Driver.hpp" // MakeDriver
#include "../../vid/Driver.hpp" // MakeDriver
#include "Window.hpp"

#ifdef DEBUG
#	include <iostream> // cout
#	include "../../log/Indenter.hpp"
#	include "../../util/x11/event.hpp" // GetEvent{Info,TypeName}
#endif

enum // _NET_WM_STATE actions
{
	_NET_WM_STATE_REMOVE,
	_NET_WM_STATE_ADD,
	_NET_WM_STATE_TOGGLE
};

namespace page
{
	namespace env
	{
		namespace x11
		{
			// construct/destroy
			Window::Window(const std::string &title)
			{
				// open display
				if (!(display = XOpenDisplay(0)))
					THROW((err::Exception<err::EnvModuleTag, err::X11PlatformTag>("failed to open display") <<
						boost::errinfo_api_function("XOpenDisplay")))
				// create window
				// FIXME: XCreateWindow expects the position of the window frame
				// while CVAR(windowPosition) is the position of the window client area
				screen = DefaultScreen(display);
				if (!(w = XCreateWindow(display, RootWindow(display, screen),
					CVAR(windowPosition).x, CVAR(windowPosition).y,
					CVAR(windowSize).x, CVAR(windowSize).y, 0,
					CopyFromParent, // depth
					InputOutput,    // class
					CopyFromParent, // visual
					0, 0)))
				{
					XCloseDisplay(display);
					THROW((err::Exception<err::EnvModuleTag, err::X11PlatformTag>("failed to create window") <<
						boost::errinfo_api_function("XCreateWindow")))
				}
				// initialize atoms
				_NET_WM_STATE            = XInternAtom(display, "_NET_WM_STATE",            False);
				_NET_WM_STATE_FULLSCREEN = XInternAtom(display, "_NET_WM_STATE_FULLSCREEN", False);
				WM_DELETE_WINDOW         = XInternAtom(display, "WM_DELETE_WINDOW",         False);
				// initialize event handling
				XSelectInput(display, w,
// FIXME: waiting for macro implementation
//#ifdef USE_X11_INPUT
					KeyPressMask |
//#endif
					FocusChangeMask | StructureNotifyMask);
				std::array<Atom, 1> protocols =
				{
					WM_DELETE_WINDOW
				};
				XSetWMProtocols(display, w, &*protocols.begin(), protocols.size());
				// set window title
				XTextProperty name =
				{
					reinterpret_cast<unsigned char *>(const_cast<char *>(title.c_str())),
					XA_STRING, 8,
					title.size()
				};
				XSetWMName(display, w, &name);
				XFlush(display);
				// initialize fullscreen mode
				if (CVAR(windowFullscreen))
				{
					try
					{
						SetFull(true);
					}
					catch (...)
					{
						CVAR(windowFullscreen) = false;
					}
				}
				// show window
				XMapWindow(display, w);
				// set initial state
				XWindowAttributes attrib;
				if (!XGetWindowAttributes(display, w, &attrib))
					THROW((err::Exception<err::EnvModuleTag, err::X11PlatformTag>("failed to get window attributes") <<
						boost::errinfo_api_function("XGetWindowAttributes")))
				// FIXME: XWindowAttributes provides the position of the window
				// frame while InitState expects the position of the window
				// client area
				InitState(true,
					math::Vector<2, int>(attrib.x, attrib.y),
					math::Vector<2, unsigned>(attrib.width, attrib.height));
			}
			Window::~Window()
			{
				Deinit();
				XDestroyWindow(display, w);
				XCloseDisplay(display);
			}

			// update
			void Window::Update()
			{
				while (XPending(display))
				{
					XEvent event;
					XNextEvent(display, &event);
#ifdef DEBUG
					if (CVAR(logVerbose))
					{
						std::cout << "X11 event: " << util::x11::GetEventTypeName(event.type) << std::endl;
						log::Indenter indenter;
						std::string eventInfo(util::x11::GetEventInfo(event));
						if (!eventInfo.empty())
							std::cout << eventInfo << std::endl;
					}
#endif
					switch (event.type)
					{
						case FocusIn: if (!HasFocus()) focusSig(true); break;
						case FocusOut: if (HasFocus()) focusSig(false); break;
						case ConfigureNotify:
						{
							// generate move event
							math::Vector<2, int> pos(event.xconfigure.x, event.xconfigure.y);
							if (Any(pos != GetPosition()))
							{
								CVAR(windowPosition) = pos;
								moveSig(pos);
							}
							// generate size event
							math::Vector<2, unsigned> size(event.xconfigure.width, event.xconfigure.height);
							if (Any(size != GetSize()))
							{
								CVAR(windowSize) = size;
								sizeSig(size);
							}
						}
						break;
						case ClientMessage:
						if (event.xclient.data.l[0] == WM_DELETE_WINDOW)
							exitSig();
						break;
					}
					eventSig(event);
				}
			}

			// platform access
			Display *Window::GetDisplay() const
			{
				return display;
			}
			int Window::GetScreen() const
			{
				return screen;
			}
			::Window Window::GetWindow() const
			{
				return w;
			}

			// environment state
			math::Vector<2, unsigned> Window::GetScreenSize() const
			{
				XWindowAttributes attrib;
				XGetWindowAttributes(display, RootWindow(display, screen), &attrib);
				return math::Vector<2, unsigned>(attrib.width, attrib.height);
			}

			// driver factory functions
			aud::Driver *Window::MakeAudioDriver()
			{
				return aud::MakeDriver(*this);
			}
			inp::Driver *Window::MakeInputDriver()
			{
				return inp::MakeDriver(*this);
			}
			vid::Driver *Window::MakeVideoDriver()
			{
				return vid::MakeDriver(*this);
			}

			// fullscreen modifiers
			void Window::SetFull(bool full)
			{
				XEvent event = {};
				event.type = ClientMessage;
				event.xclient.display = display;
				event.xclient.window = w;
				event.xclient.message_type = _NET_WM_STATE;
				event.xclient.format = 32;
				event.xclient.data.l[0] = full ? _NET_WM_STATE_ADD : _NET_WM_STATE_REMOVE;
				event.xclient.data.l[1] = _NET_WM_STATE_FULLSCREEN;
				event.xclient.data.l[3] = 1;
				if (!XSendEvent(display, w, False, SubstructureNotifyMask | SubstructureRedirectMask, &event))
					THROW((err::Exception<err::EnvModuleTag, err::X11PlatformTag>("failed to send event") <<
						boost::errinfo_api_function("XSendEvent")))
			}
			void Window::SwitchFull()
			{
				// FIXME: implement
			}
		}

		// factory function
		Window *MakeWindow(const std::string &title)
			{ return new x11::Window(title); }
	}
}
