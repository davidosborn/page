/**
 * @section license
 *
 * Copyright (c) 2006-2013 David Osborn
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

#ifndef    page_local_wnd_win32_Window_hpp
#   define page_local_wnd_win32_Window_hpp

#	include <windows.h> // HWND, LPARAM, LRESULT, UINT, WPARAM

#	include "../Window.hpp"

namespace page { namespace wnd { namespace win32
{
	class Window :
		public wnd::Window,
		public virtual util::Cloneable<Window, wnd::Window>
	{
		public:
		// construct/destroy
		explicit Window(const std::string &title);
		~Window();

		// update
		void Update();

		// platform access
		HWND GetHwnd() const;

		// message signal
		boost::signal<void (UINT, WPARAM, LPARAM)> messageSig;

		// environment state
		math::Vector<2, unsigned> GetScreenSize() const;

		private:
		// driver factory functions
		aud::Driver *MakeAudioDriver();
		inp::Driver *MakeInputDriver();
		vid::Driver *MakeVideoDriver();

		// fullscreen modifiers
		static void SetFull(bool);
		void SwitchFull();

		// message handling
		LRESULT WindowProc(UINT, WPARAM, LPARAM);
		static LRESULT CALLBACK MsgRouter(HWND, UINT, WPARAM, LPARAM);

		// window class
		static LPCTSTR MakeClass();
		static LPCTSTR GetClass();

		HWND hwnd;
		bool
			alive, min, full,              // window state
			waitFocus, moving, sizing,     // transition state
			muteFocus, muteMove, muteSize; // hide unwanted messages
		HBITMAP clientBitmap; // for saving client area
	};
}}}

#endif
