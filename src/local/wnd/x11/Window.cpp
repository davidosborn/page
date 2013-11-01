#include <array>

#include <X11/Xatom.h> // XA_STRING
#include <X11/Xutil.h> // XSetWMName, XTextProperty

#include "../../cfg/vars.hpp"
#include "../../err/Exception.hpp"
#include "../../util/cpp.hpp" // STRINGIZE
#include "../WindowRegistry.hpp" // REGISTER_WINDOW
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

namespace page { namespace wnd { namespace x11
{
	/*-------------+
	| constructors |
	+-------------*/

	Window::Window(const std::string &title)
	{
		// open display
		if (!(display = XOpenDisplay(0)))
			THROW((err::Exception<err::EnvModuleTag, err::X11PlatformTag>("failed to open display") <<
				boost::errinfo_api_function("XOpenDisplay")))
		// create window
		// FIXME: XCreateWindow expects the position of the window frame
		// while *CVAR(windowPosition) is the position of the window client area
		screen = DefaultScreen(display);
		if (!(w = XCreateWindow(display, RootWindow(display, screen),
			CVAR(windowPosition)->x, CVAR(windowPosition)->y,
			CVAR(windowSize)->x, CVAR(windowSize)->y, 0,
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
		if (*CVAR(windowFullscreen))
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
			math::Vec2i(attrib.x, attrib.y),
			math::Vec2u(attrib.width, attrib.height));
	}

	Window::~Window()
	{
		Deinit();
		XDestroyWindow(display, w);
		XCloseDisplay(display);
	}

	/*--------------------+
	| copy/move semantics |
	+--------------------*/

	Window::Window(const Window &other)
	{
		// FIXME: create new window with same title, position, and size as other
	}

	Window &Window::operator =(Window other)
	{
		std::swap(*this, other);
		return *this;
	}

	// update
	void Window::Update()
	{
		while (XPending(display))
		{
			XEvent event;
			XNextEvent(display, &event);
#ifdef DEBUG
			if (*CVAR(logVerbose))
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
					math::Vec2i pos(event.xconfigure.x, event.xconfigure.y);
					if (Any(pos != GetPosition()))
					{
						CVAR(windowPosition) = pos;
						moveSig(pos);
					}
					// generate size event
					math::Vec2u size(event.xconfigure.width, event.xconfigure.height);
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
	math::Vec2u Window::GetScreenSize() const
	{
		XWindowAttributes attrib;
		XGetWindowAttributes(display, RootWindow(display, screen), &attrib);
		return math::Vec2u(attrib.width, attrib.height);
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

	REGISTER_WINDOW(Window, STRINGIZE(X11_NAME) " window")
}}}
