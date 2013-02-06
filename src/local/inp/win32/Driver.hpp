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

#ifndef    page_local_inp_win32_Driver_hpp
#   define page_local_inp_win32_Driver_hpp

#	include <windows.h> // DWORD, HWND, LONG, LPARAM, UINT{,_PTR}, WPARAM
#	include "../Driver.hpp"

namespace page
{
	namespace env {namespace win32 { class Window; }}

	namespace inp
	{
		namespace win32
		{
			struct Driver : inp::Driver
			{
				// construct
				explicit Driver(env::win32::Window &);

				// window access
				env::win32::Window &GetWindow();
				const env::win32::Window &GetWindow() const;

				private:
				// state query
				State Poll() const;

				// cursor mode modifiers
				void DoSetCursorMode(CursorMode);
				void ShowCursor(bool);

				// system cursor state
				math::Vector<2, unsigned> GetRawCursorPosition() const;
				math::Vector<2, int> GetRawCursorTranslation() const;
				bool GetMouseButton(Button) const;

				// cursor update
				void UpdateCursor();

				// inspiration notification
				void OnImbue(const ui::Interface *);

				// window signal handlers
				void OnFocus(bool focus);
				void OnMessage(UINT, WPARAM, LPARAM);
				void OnMove(const math::Vector<2, int> &);
				void OnSize(const math::Vector<2, unsigned> &);
				util::ScopedConnection focusCon, messageCon, moveCon, sizeCon;

				// window message handlers
				void OnButtonDown(Button, WPARAM, LPARAM);
				void OnButtonUp(Button, WPARAM, LPARAM);
				void OnMouseMove(WPARAM, LPARAM);
				void OnMouseWheel(WPARAM, LPARAM);

				// mouse state
				void ResetMouseState();
				void ResetMouseState(const math::Vector<2> &position);
				void ResetMouseDrag(const math::Vector<2> &position);

				// mouse repeat timer
				void StartMouseRepeatTimer();
				void StopMouseRepeatTimer();

				// mouse repeat timer handling
				void OnMouseRepeatTimer();
				static void CALLBACK MouseRepeatTimerRouter(HWND, UINT, UINT_PTR, DWORD);

				// vector normalization
				math::Vector<2> NormClientVector(const math::Vector<2, int> &) const;
				math::Vector<2> NormScreenVector(const math::Vector<2, int> &) const;

				// MSH_MOUSEWHEEL message
				UINT mshMousewheel;

				// system limits
				struct Limits
				{
					// construct
					Limits();

					UINT doubleTime;
					math::Vector<2, unsigned> doubleRange, dragRange;
					unsigned repeatDelay;
					DWORD repeatSpeed;
				} limits;

				// cursor state
				struct Cursor
				{
					// construct
					Cursor();

					enum Icon
					{
						noIcon,
						arrowIcon,
						themeIcon
					} icon;
					bool visible;
					math::Vector<2, unsigned> pointPosition;
				} cursor;

				// mouse state
				struct Mouse
				{
					// construct
					Mouse();

					bool down, dragging, _double, ignoreDown;
					Button downButton;
					LONG downTime;
					math::Vector<2, unsigned> downPosition;
					float deltaScroll;
					// repeating
					bool repeating;
					UINT_PTR repeatTimer;
				} mouse;
			};
		}
	}
}

#endif
