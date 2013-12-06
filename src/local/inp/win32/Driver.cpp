#include <cassert>
#include <cctype> // isprint
#include <cstdlib> // abs
#include <functional> // bind
#include <unordered_map>

#include <windows.h>

#include "../../cache/proxy/win32/CursorProxy.hpp"
#include "../../err/Exception.hpp"
#include "../../math/Aabb.hpp"
#include "../../math/win32.hpp" // Make{Rect,Vector}
#include "../../wnd/win32/Window.hpp" // REGISTER_DRIVER, Window->wnd::Window
#include "../DriverRegistry.hpp" // REGISTER_DRIVER
#include "Driver.hpp"
#include "SystemSettings.hpp"

namespace page { namespace inp { namespace win32
{
	namespace
	{
		// mouse repeat timer to driver mapping
		typedef std::unordered_map<UINT_PTR, Driver *> MouseRepeatTimerDriverMap;
		inline MouseRepeatTimerDriverMap &GetMouseRepeatTimerDriverMap()
		{
			static MouseRepeatTimerDriverMap map;
			return map;
		}
	}

	/*-------------+
	| constructors |
	+-------------*/

	Driver::Driver(wnd::win32::Window &wnd) : inp::Driver(wnd)
	{
		// connect signals
		using std::bind;
		using namespace std::placeholders;
		focusCon   = wnd.focusSig  .connect(bind(&Driver::OnFocus,   this, _1));
		messageCon = wnd.messageSig.connect(bind(&Driver::OnMessage, this, _1, _2, _3));
		moveCon    = wnd.moveSig   .connect(bind(&Driver::OnMove,    this, _1));
		sizeCon    = wnd.sizeSig   .connect(bind(&Driver::OnSize,    this, _1));

		// register MSH_MOUSEWHEEL message for MSWheel module
		mshMousewheel = RegisterWindowMessage(TEXT("MSWHEEL_ROLLMSG"));

		// initialize cursor
		RefreshCursor();
	}

	/*--------------+
	| window access |
	+--------------*/

	wnd::win32::Window &Driver::GetWindow()
	{
		return static_cast<wnd::win32::Window &>(inp::Driver::GetWindow());
	}

	const wnd::win32::Window &Driver::GetWindow() const
	{
		return static_cast<const wnd::win32::Window &>(inp::Driver::GetWindow());
	}

	/*---------------------------+
	| inp::Driver implementation |
	+---------------------------*/

	void Driver::DoSetCursorMode(CursorMode mode)
	{
		ShowCursor(mode == CursorMode::point);
	}

	void Driver::DoSetCursor(const cache::Proxy<res::Cursor> &cursor)
	{
		RefreshCursor();
	}

	PollState Driver::Poll() const
	{
		PollState state;

		// control
		state.control.direction = math::Vec2(
				((GetAsyncKeyState(VK_RIGHT) & 0x8000) >> 15) -
				((GetAsyncKeyState(VK_LEFT)  & 0x8000) >> 15),
				((GetAsyncKeyState(VK_UP)    & 0x8000) >> 15) -
				((GetAsyncKeyState(VK_DOWN)  & 0x8000) >> 15));
		state.control.modifiers |= Modifier::run & bool(GetAsyncKeyState(VK_SHIFT)   & 0x8000);
		state.control.modifiers |= Modifier::alt & bool(GetAsyncKeyState(VK_CONTROL) & 0x8000);

		// look
		if (GetCursorMode() == CursorMode::look)
		{
			math::Vec2 translation(
				math::Vec2(GetRawCursorTranslation()) /
				GetWindow().GetScreenSize());
			if (GetMouseButton(Button::left))
			{
				state.look.lift = translation.y;
			}
			else if (GetMouseButton(Button::right))
			{
				state.look.zoom = translation.y;
			}
			else
			{
				state.look.rotation.yaw   = translation.x;
				state.look.rotation.pitch = translation.y;
			}
			state.look.zoom += mouseState.deltaScroll;
			const_cast<Driver &>(*this).mouseState.deltaScroll = 0;
		}

		// events
		// FIXME: we should be putting key/char events here, rather than
		// firing them off directly from OnMessage

		return state;
	}

	math::Vec2u Driver::GetRawCursorPosition() const
	{
		POINT pt;
		GetCursorPos(&pt);
		return math::win32::MakeVector(pt);
	}

	/*---------------+
	| implementation |
	+---------------*/

	void Driver::ShowCursor(bool show)
	{
		if (show == cursorState.visible) return;
		if (show)
		{
			ClipCursor(NULL);
			SetCursorPos(
				cursorState.pointPosition.x,
				cursorState.pointPosition.y);
			::ShowCursor(TRUE);
		}
		else
		{
			ResetMouseState();
			cursorState.pointPosition = Driver::GetRawCursorPosition();
			::ShowCursor(FALSE);
			auto box(
				AabbPositionSize(
					GetWindow().GetPosition(),
					math::Vec2i(GetWindow().GetSize())));
			auto center(Center(Shrink(box, 0, 1)));
			SetCursorPos(center.x, center.y);
			auto rect(math::win32::MakeRect(box));
			ClipCursor(&rect);
		}
		cursorState.visible = show;
	}

	void Driver::RefreshCursor()
	{
		if (GetWindow().HasFocus() &&
			ContainsMaxExclusive(
				AabbPositionSize(
					GetWindow().GetPosition(),
					math::Vec2i(GetWindow().GetSize())),
				GetRawCursorPosition()))
		{
			cursorState.icon = CursorState::Icon::none;
			PostMessage(GetWindow().GetHwnd(), WM_SETCURSOR,
				reinterpret_cast<WPARAM>(GetWindow().GetHwnd()),
				MAKELPARAM(HTCLIENT, WM_MOUSEMOVE));
		}
	}

	math::Vec2i Driver::GetRawCursorTranslation() const
	{
		POINT pt;
		GetCursorPos(&pt);
		auto box(
			AabbPositionSize(
				GetWindow().GetPosition(),
				math::Vec2i(GetWindow().GetSize())));
		auto center(Center(Shrink(box, 0, 1)));
		SetCursorPos(center.x, center.y);
		return math::win32::MakeVector(pt) - center;
	}

	bool Driver::GetMouseButton(Button button) const
	{
		// redirect requested button according to system settings
		if (GetSystemMetrics(SM_SWAPBUTTON))
			switch (button)
			{
				case Button::left:  button = Button::right;
				case Button::right: button = Button::left;
			}

		// return current state of requested button
		int key;
		switch (button)
		{
			case Button::left:   key = VK_LBUTTON; break;
			case Button::middle: key = VK_MBUTTON; break;
			case Button::right:  key = VK_RBUTTON; break;
			default: assert(!"invalid mouse button");
		}
		return GetAsyncKeyState(key) & 0x8000;
	}

	/*-----------------------------------+
	| wnd::win32::Window signal handlers |
	+-----------------------------------*/

	void Driver::OnFocus(bool focus)
	{
		if (GetCursorMode() == CursorMode::point)
		{
			if (focus)
			{
				// ignore focus changing mouse clicks
				MSG msg;
				PeekMessage(&msg, GetWindow().GetHwnd(), WM_MOUSEFIRST, WM_MOUSELAST, PM_NOREMOVE);
				switch (msg.message)
				{
					case WM_LBUTTONDOWN:
					case WM_MBUTTONDOWN:
					case WM_RBUTTONDOWN:
					mouseState.ignoreDown = true;
					break;
				}
			}
			else ResetMouseState();
		}
		else ShowCursor(!focus);
	}

	void Driver::OnMessage(UINT msg, WPARAM wparam, LPARAM lparam)
	{
		switch (msg)
		{
			case WM_SETTINGCHANGE:
			GLOBAL(SystemSettings).Update();
			break;

			// cursor messages
			case WM_SETCURSOR:
			case WM_DISPLAYCHANGE:
			if (cursorState.visible && LOWORD(lparam) == HTCLIENT)
			{
				if (GetWindow().HasFocus())
				{
					if (auto cursor = GetCursor())
					{
						::SetCursor(*cache::win32::CursorProxy(cursor, GetWindow().GetScreenSize().y));
						cursorState.icon = CursorState::Icon::theme;
					}
					else goto DefaultCursor;
				}
				else
				{
					DefaultCursor:
					::SetCursor(LoadCursor(NULL, IDC_ARROW));
					cursorState.icon = CursorState::Icon::arrow;
				}
			}
			else cursorState.icon = CursorState::Icon::none;
			break;

			// keyboard messages
			case WM_CHAR:
			if (std::isprint(wparam)) charSig(wparam);
			break;

			case WM_KEYDOWN:
			if (wparam == VK_ESCAPE &&
				GetCursorMode() == CursorMode::point && mouseState.down)
				ResetMouseState(NormScreenVector(math::win32::MakeVector(GetMessagePos())));
			else
			{
				Key key;
				switch (wparam)
				{
					case VK_BACK:   key = Key::backspace; break;
					case VK_DELETE: key = Key::delete_;   break;
					case VK_DOWN:   key = Key::down;      break;
					case VK_RETURN: key = Key::enter;     break;
					case VK_ESCAPE: key = Key::escape;    break;
					case VK_LEFT:   key = Key::left;      break;
					case VK_PAUSE:  key = Key::pause;     break;
					case VK_F12:    key = Key::print;     break;
					case VK_F9:     key = Key::record;    break;
					case VK_RIGHT:  key = Key::right;     break;
					case VK_TAB:    key = Key::tab;       break;
					case VK_UP:     key = Key::up;        break;
					default: goto NoKey;
				}
				keySig(key);
				NoKey:;
			}
			break;

			// mouse messages
			case WM_LBUTTONDOWN: OnButtonDown(Button::left,   wparam, lparam); break;
			case WM_MBUTTONDOWN: OnButtonDown(Button::middle, wparam, lparam); break;
			case WM_RBUTTONDOWN: OnButtonDown(Button::right,  wparam, lparam); break;
			case WM_LBUTTONUP:   OnButtonUp  (Button::left,   wparam, lparam); break;
			case WM_MBUTTONUP:   OnButtonUp  (Button::middle, wparam, lparam); break;
			case WM_RBUTTONUP:   OnButtonUp  (Button::right,  wparam, lparam); break;
			case WM_MOUSEMOVE:   OnMouseMove (wparam, lparam); break;
			case WM_MOUSEWHEEL:  OnMouseWheel(wparam, lparam); break;
		}
		if (msg == mshMousewheel) OnMouseWheel(wparam, lparam);
	}

	void Driver::OnMove(const math::Vec2i &position)
	{
		if (GetCursorMode() == CursorMode::look)
		{
			// update cursor clipping
			ClipCursor(NULL);
			auto box(
				AabbPositionSize(
					position,
					math::Vec2i(GetWindow().GetSize())));
			auto center(Center(Shrink(box, 0, 1)));
			SetCursorPos(center.x, center.y);
			auto rect(math::win32::MakeRect(box));
			ClipCursor(&rect);
		}
	}

	void Driver::OnSize(const math::Vec2u &size)
	{
		if (GetCursorMode() == CursorMode::look)
		{
			// update cursor clipping
			ClipCursor(NULL);
			auto box(
				AabbPositionSize(
					GetWindow().GetPosition(),
					math::Vec2i(size)));
			auto center(Center(Shrink(box, 0, 1)));
			SetCursorPos(center.x, center.y);
			auto rect(math::win32::MakeRect(box));
			ClipCursor(&rect);
		}
	}

	/*------------------------+
	| window message handlers |
	+------------------------*/

	void Driver::OnButtonDown(Button button, WPARAM wparam, LPARAM lparam)
	{
		if (GetCursorMode() != CursorMode::point) return;
		if (mouseState.ignoreDown)
		{
			mouseState.ignoreDown = false;
			return;
		}
		SetCapture(GetWindow().GetHwnd());
		StartMouseRepeatTimer();

		// cancel dragging
		auto position(math::win32::MakeVector(lparam));
		auto normPosition(NormClientVector(position));
		ResetMouseDrag(normPosition);

		// signal down
		auto time = GetMessageTime();
		mouseState._double =
			button == mouseState.downButton &&
			time - mouseState.downTime <= GLOBAL(SystemSettings).GetDoubleTime() &&
			All(Abs(
				math::Vec2i(position) -
				math::Vec2i(mouseState.downPosition)) <= GLOBAL(SystemSettings).GetDoubleThreshold());
		downSig(normPosition, button, mouseState._double);

		// update mouse state
		mouseState.down = true;
		mouseState.downButton = button;
		mouseState.downTime = time;
		mouseState.downPosition = position;
	}

	void Driver::OnButtonUp(Button button, WPARAM wparam, LPARAM lparam)
	{
		if (GetCursorMode() != CursorMode::point) return;
		if (mouseState.down && button == mouseState.downButton)
		{
			ReleaseCapture();
			StopMouseRepeatTimer();

			// signal click/drop
			math::Vec2u position(math::win32::MakeVector(lparam));
			math::Vec2 normPosition(NormClientVector(position));
			if (mouseState.dragging)
			{
				dropSig(
					NormClientVector(mouseState.downPosition),
					normPosition, button, mouseState._double);
				mouseState.dragging = false;
				mouseState.downTime = 0;
			}
			else clickSig(normPosition, button, mouseState._double);

			// update mouse state
			mouseState.down = false;
		}
	}

	void Driver::OnMouseMove(WPARAM wparam, LPARAM lparam)
	{
		if (GetCursorMode() != CursorMode::point) return;
		if (mouseState.down && !mouseState.dragging)
		{
			// check for dragging
			math::Vec2u position(math::win32::MakeVector(lparam));
			if (Any(Abs(
					math::Vec2i(position) -
					math::Vec2i(mouseState.downPosition)) >= GLOBAL(SystemSettings).GetDragThreshold()))
			{
				StopMouseRepeatTimer();
				dragSig(
					NormClientVector(mouseState.downPosition),
					mouseState.downButton, mouseState._double);
				mouseState.dragging = true;
			}
		}
	}

	void Driver::OnMouseWheel(WPARAM wparam, LPARAM lparam)
	{
		if (!wparam) return;
		auto delta = static_cast<float>(GET_WHEEL_DELTA_WPARAM(wparam)) / WHEEL_DELTA;
		switch (GetCursorMode())
		{
			case CursorMode::point:
			scrollSig(NormScreenVector(math::win32::MakeVector(lparam)), delta);
			break;

			case CursorMode::look:
			mouseState.deltaScroll += delta;
			break;
		}
	}

	/*------------+
	| mouse state |
	+------------*/

	void Driver::ResetMouseState()
	{
		ResetMouseState(GetCursorPosition());
	}

	void Driver::ResetMouseState(const math::Vec2 &position)
	{
		assert(GetCursorMode() == CursorMode::point);
		if (mouseState.dragging) ResetMouseDrag(position);
		else StopMouseRepeatTimer();
		mouseState.down = false;
		mouseState.downTime = 0;
	}

	void Driver::ResetMouseDrag(const math::Vec2 &position)
	{
		assert(GetCursorMode() == CursorMode::point);
		if (mouseState.dragging)
		{
			cancelDragSig(
				NormClientVector(mouseState.downPosition),
				position, mouseState.downButton, mouseState._double);
			mouseState.dragging = false;
			mouseState.downTime = 0;
		}
	}

	/*-------------------+
	| mouse repeat timer |
	+-------------------*/

	void Driver::StartMouseRepeatTimer()
	{
		mouseState.repeating = false;
		mouseState.repeatTimer = SetTimer(0, mouseState.repeatTimer, GLOBAL(SystemSettings).GetRepeatDelay(), &Driver::MouseRepeatTimerRouter);
		GetMouseRepeatTimerDriverMap().insert(std::make_pair(mouseState.repeatTimer, this));
	}

	void Driver::StopMouseRepeatTimer()
	{
		if (mouseState.repeatTimer)
		{
			KillTimer(NULL, mouseState.repeatTimer);
			GetMouseRepeatTimerDriverMap().erase(mouseState.repeatTimer);
			mouseState.repeatTimer = 0;
		}
	}

	void Driver::OnMouseRepeatTimer()
	{
		if (!mouseState.repeating)
		{
			mouseState.repeating = true;
			SetTimer(0, mouseState.repeatTimer, GLOBAL(SystemSettings).GetRepeatSpeed(), &Driver::MouseRepeatTimerRouter);
		}
		downSig(
			NormScreenVector(math::win32::MakeVector(GetMessagePos())),
			mouseState.downButton, mouseState._double);
	}

	void CALLBACK Driver::MouseRepeatTimerRouter(HWND, UINT, UINT_PTR timer, DWORD)
	{
		auto iter(GetMouseRepeatTimerDriverMap().find(timer));
		if (iter != GetMouseRepeatTimerDriverMap().end())
		{
			auto &driver(*iter->second);
			driver.OnMouseRepeatTimer();
		}
	}

	/*-------------------------+
	| coordinate normalization |
	+-------------------------*/

	math::Vec2 Driver::NormClientVector(const math::Vec2i &v) const
	{
		return Min(Max(EnterSpace(math::Aabb<2, int>(0, (GetWindow().GetSize() - 1)), v), 0), 1);
	}

	math::Vec2 Driver::NormScreenVector(const math::Vec2i &v) const
	{
		return NormClientVector(v - GetWindow().GetPosition());
	}

	/*-------------+
	| registration |
	+-------------*/

	REGISTER_DRIVER(Driver, wnd::win32::Window, STRINGIZE(WIN32_NAME) " input driver")
}}}
