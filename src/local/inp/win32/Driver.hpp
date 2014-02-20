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

#ifndef    page_local_inp_win32_Driver_hpp
#   define page_local_inp_win32_Driver_hpp

#	include <windows.h> // DWORD, HWND, LONG, LPARAM, UINT{,_PTR}, WPARAM

#	include "../Driver.hpp"

namespace page { namespace wnd { namespace win32 { class Window; }}}

namespace page { namespace inp { namespace win32
{
	/**
	 * The Win32 input driver.
	 *
	 * @note The process must have WINSTA_WRITEATTRIBUTES access to use the
	 * ::ClipCursor() and ::SetCursor() functions.
	 */
	class Driver : public inp::Driver
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit Driver(wnd::win32::Window &);

		/*--------------+
		| window access |
		+--------------*/

		wnd::win32::Window &GetWindow();
		const wnd::win32::Window &GetWindow() const;

		/*---------------------------+
		| inp::Driver implementation |
		+---------------------------*/

		private:
		void DoSetCursorMode(CursorMode) override;
		void DoSetCursor(const cache::Proxy<res::Cursor> &) override;
		PollState Poll() const override;
		math::Vec2u GetRawCursorPosition() const override;

		/*---------------+
		| implementation |
		+---------------*/

		/**
		 * Shows or hides the cursor.
		 */
		void ShowCursor(bool);

		/**
		 * Refreshes the cursor's appearance, which depends on its visibility
		 * and the currently set cursor resource.
		 */
		void RefreshCursor();

		/**
		 * Returns the cursor's translation (change in position) in pixels since
		 * the last time the function was called, for CursorMode::look.
		 *
		 * If possible, the hardware cursor is moved to the center of the screen
		 * to allow for maximum translation during the next frame.
		 */
		math::Vec2i GetRawCursorTranslation() const;

		/**
		 * Returns @c true if the specified mouse button is down.
		 */
		bool GetMouseButton(Button) const;

		/*-----------------------------------+
		| wnd::win32::Window signal handlers |
		+-----------------------------------*/

		void OnFocus(bool focus);
		void OnMessage(UINT, WPARAM, LPARAM);
		void OnMove(const math::Vec2i &);
		void OnSize(const math::Vec2u &);
		boost::signals::scoped_connection focusCon, messageCon, moveCon, sizeCon;

		/*------------------------+
		| window message handlers |
		+------------------------*/

		void OnButtonDown(Button, WPARAM, LPARAM);
		void OnButtonUp(Button, WPARAM, LPARAM);
		void OnMouseMove(WPARAM, LPARAM);
		void OnMouseWheel(WPARAM, LPARAM);

		/*------------+
		| mouse state |
		+------------*/

		/**
		 * Clears the mouse of any intermediate state.
		 */
		void ResetMouseState();

		/**
		 * Clears the mouse of any intermediate state.
		 */
		void ResetMouseState(const math::Vec2 &position);

		/**
		 * Clears the mouse of any intermediate dragging state.
		 */
		void ResetMouseDrag(const math::Vec2 &position);

		/*-------------------+
		| mouse repeat timer |
		+-------------------*/

		/**
		 * Starts the mouse-repeat timer.
		 */
		void StartMouseRepeatTimer();

		/**
		 * Stops the mouse-repeat timer.
		 */
		void StopMouseRepeatTimer();

		/**
		 * The callback for the mouse repeat timer.
		 */
		void OnMouseRepeatTimer();

		/**
		 * The routing callback-function for the mouse-repeat timer, passed to
		 * ::SetTimer(), which defers to OnMouseRepeatTimer() through the
		 * appropriate driver instance.
		 */
		static void CALLBACK MouseRepeatTimerRouter(HWND, UINT, UINT_PTR, DWORD);

		/*-------------------------+
		| coordinate normalization |
		+-------------------------*/

		/**
		 * Normalizes client-space coordinates in pixels to units in the range
		 * @f$[0,1]@f$.
		 */
		math::Vec2 NormClientVector(const math::Vec2i &) const;

		/**
		 * Normalizes screen-space coordinates in pixels to client-space units
		 * in the range @f$[0,1]@f$.
		 */
		math::Vec2 NormScreenVector(const math::Vec2i &) const;

		/*-------------+
		| data members |
		+-------------*/

		/**
		 * The MSH_MOUSEWHEEL message ID.
		 */
		UINT mshMousewheel;

		/**
		 * Cursor state.
		 */
		struct CursorState
		{
			enum class Icon
			{
				none,
				arrow,
				theme
			} icon = Icon::none;
			bool visible = true;
			math::Vec2u pointPosition;
		} cursorState;

		/**
		 * Mouse state.
		 */
		struct MouseState
		{
			/**
			 * @c true if at least one mouse button is being pressed.
			 */
			bool down = false;

			/**
			 * @c true if the mouse is currently in a dragging state.  The
			 * dragging state will be entered when a mouse button is held down
			 * and the cursor is moved beyond a certain threshold.
			 */
			bool dragging = false;

			/**
			 * @c true if the same mouse button was pressed, released, and
			 * pressed again within a certain timeframe, which would signal a
			 * double-click or double-drag operation.
			 */
			bool _double = false;

			/**
			 * @c true if mouse-button presses should be ignored.
			 */
			bool ignoreDown = false;

			/**
			 * The mouse button that was last pressed.
			 */
			Button downButton;

			/**
			 * The time when the mouse button was last pressed.
			 */
			LONG downTime;

			/**
			 * The cursor position when the mouse button was last pressed.
			 */
			math::Vec2u downPosition;

			/**
			 * The amount that the scroll wheel has turned since the last frame,
			 * in mouse-wheel units.
			 */
			float deltaScroll = 0;

			/**
			 * @defgroup mouse-button-repeating
			 * @{
			 */
			/**
			 * @c true if the last mouse-button press is currently in a
			 * repeating state.  The repeating state will be entered when a
			 * mouse button is held down beyond a certain period of time.
			 */
			bool repeating = false;

			/**
			 * The timer used to implement mouse-button repeating.
			 */
			UINT_PTR repeatTimer = 0;
			///@}
		} mouseState;
	};
}}}

#endif
