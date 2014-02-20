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

#ifndef    page_local_wnd_x11_Window_hpp
#   define page_local_wnd_x11_Window_hpp

#	include <string>

#	include <X11/Xlib.h> // Atom, Display, Window, XEvent

#	include "../Window.hpp"

namespace page { namespace wnd { namespace x11
{
	class Window : public wnd::Window
	{
		IMPLEMENT_CLONEABLE(Window, wnd::Window)

		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit Window(const std::string &title);
		~Window() override;

		/*--------------------+
		| copy/move semantics |
		+--------------------*/

		Window(const Window &);
		Window &operator =(const Window &);

		// update
		void Update();

		// platform access
		Display *GetDisplay() const;
		int GetScreen() const;
		::Window GetWindow() const;

		// event signal
		boost::signal<void (const XEvent &)> eventSig;

		// environment state
		math::Vec2u GetScreenSize() const;

		private:
		// fullscreen modifiers
		void SetFull(bool);
		void SwitchFull();

		// atoms
		Atom
			_NET_WM_STATE,
			_NET_WM_STATE_FULLSCREEN,
			WM_DELETE_WINDOW;

		Display *display;
		int screen;
		::Window w;
	};
}}}

#endif
