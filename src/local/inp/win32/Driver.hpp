#ifndef    page_local_inp_win32_Driver_hpp
#   define page_local_inp_win32_Driver_hpp

#	include <windows.h> // DWORD, HWND, LONG, LPARAM, UINT{,_PTR}, WPARAM

#	include "../Driver.hpp"

namespace page
{
	namespace wnd { namespace win32 { class Window; }}

	namespace inp
	{
		namespace win32
		{
			struct Driver : inp::Driver
			{
				// construct
				explicit Driver(wnd::win32::Window &);

				// window access
				wnd::win32::Window &GetWindow();
				const wnd::win32::Window &GetWindow() const;

				private:
				// state query
				State Poll() const;

				// cursor mode modifiers
				void DoSetCursorMode(CursorMode);
				void ShowCursor(bool);

				// system cursor state
				math::Vec2u GetRawCursorPosition() const;
				math::Vec2i GetRawCursorTranslation() const;
				bool GetMouseButton(Button) const;

				// cursor update
				void UpdateCursor();

				// inspiration notification
				void OnImbue(const ui::UserInterface *);

				// window signal handlers
				void OnFocus(bool focus);
				void OnMessage(UINT, WPARAM, LPARAM);
				void OnMove(const math::Vec2i &);
				void OnSize(const math::Vec2u &);
				boost::signals::scoped_connection focusCon, messageCon, moveCon, sizeCon;

				// window message handlers
				void OnButtonDown(Button, WPARAM, LPARAM);
				void OnButtonUp(Button, WPARAM, LPARAM);
				void OnMouseMove(WPARAM, LPARAM);
				void OnMouseWheel(WPARAM, LPARAM);

				// mouse state
				void ResetMouseState();
				void ResetMouseState(const math::Vec2 &position);
				void ResetMouseDrag(const math::Vec2 &position);

				// mouse repeat timer
				void StartMouseRepeatTimer();
				void StopMouseRepeatTimer();

				// mouse repeat timer handling
				void OnMouseRepeatTimer();
				static void CALLBACK MouseRepeatTimerRouter(HWND, UINT, UINT_PTR, DWORD);

				// vector normalization
				math::Vec2 NormClientVector(const math::Vec2i &) const;
				math::Vec2 NormScreenVector(const math::Vec2i &) const;

				// MSH_MOUSEWHEEL message
				UINT mshMousewheel;

				// system limits
				struct Limits
				{
					// construct
					Limits();

					UINT doubleTime;
					math::Vec2u doubleRange, dragRange;
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
					math::Vec2u pointPosition;
				} cursor;

				// mouse state
				struct Mouse
				{
					// construct
					Mouse();

					bool down, dragging, _double, ignoreDown;
					Button downButton;
					LONG downTime;
					math::Vec2u downPosition;
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
