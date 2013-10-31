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
		 * The system's current configuration.
		 *
		 * @todo This should be moved into a static singleton, so that multiple
		 * input drivers don't have to maintain separate copies of the same
		 * system settings.
		 */
		struct SystemSettings
		{
			Limits();

			UINT doubleTime;
			math::Vec2u doubleRange, dragRange;
			unsigned repeatDelay;
			DWORD repeatSpeed;
		} systemSettings;

		/**
		 * Cursor state.
		 */
		struct CursorState
		{
			enum Icon
			{
				noIcon,
				arrowIcon,
				themeIcon
			} icon = noIcon;
			bool visible = true;
			math::Vec2u pointPosition;
		} cursorState;

		/**
		 * Mouse state.
		 */
		struct MouseState
		{
			bool down = false;
			bool dragging = false;
			bool _double = false;
			bool ignoreDown = false;
			Button downButton = 0;
			LONG downTime;
			math::Vec2u downPosition;
			float deltaScroll = 0;

			// repeating
			bool repeating = false;
			UINT_PTR repeatTimer = 0;
		} mouseState;
	};
}}}

#endif
