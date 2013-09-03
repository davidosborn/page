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
#include "../../res/type/Theme.hpp" // Theme::cursor
#include "../../gui/UserInterface.hpp" // UserInterface::GetTheme
#include "../../wnd/win32/Window.hpp" // Window->wnd::Window, Window::GetHwnd
#include "../../wnd/Window.hpp" // Window::{{focus,message,move,size}Sig,Get{Position,Size},HasFocus}
#include "Driver.hpp"

namespace page
{
	namespace inp
	{
		namespace win32
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

			// construct
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
				UpdateCursor();
			}

			// window access
			wnd::win32::Window &Driver::GetWindow()
			{
				return static_cast<wnd::win32::Window &>(inp::Driver::GetWindow());
			}
			const wnd::win32::Window &Driver::GetWindow() const
			{
				return static_cast<const wnd::win32::Window &>(inp::Driver::GetWindow());
			}

			// state query
			State Driver::Poll() const
			{
				State state;
				// control
				state.control.direction = math::Vec2(
						((GetAsyncKeyState(VK_RIGHT) & 0x8000) >> 15) -
						((GetAsyncKeyState(VK_LEFT)  & 0x8000) >> 15),
						((GetAsyncKeyState(VK_UP)    & 0x8000) >> 15) -
						((GetAsyncKeyState(VK_DOWN)  & 0x8000) >> 15));
				state.control.modifiers[runModifier] = GetAsyncKeyState(VK_SHIFT)   & 0x8000;
				state.control.modifiers[altModifier] = GetAsyncKeyState(VK_CONTROL) & 0x8000;
				// look
				if (GetCursorMode() == lookCursorMode)
				{
					math::Vec2 translation(
						math::Vec2(GetRawCursorTranslation()) /
						GetWindow().GetScreenSize());
					if (GetMouseButton(leftButton))
					{
						state.look.lift = translation.y;
					}
					else if (GetMouseButton(rightButton))
					{
						state.look.zoom = translation.y;
					}
					else
					{
						state.look.rotation.yaw   = translation.x;
						state.look.rotation.pitch = translation.y;
					}
					state.look.zoom += mouse.deltaScroll;
					const_cast<Driver &>(*this).mouse.deltaScroll = 0;
				}
				// events
				// FIXME: we should be putting key/char events here, rather than
				// firing them off directly from OnMessage
				return state;
			}

			// cursor mode modifiers
			void Driver::DoSetCursorMode(CursorMode mode)
			{
				ShowCursor(mode == pointCursorMode);
			}
			void Driver::ShowCursor(bool show)
			{
				if (show == cursor.visible) return;
				// NOTE: {ClipCursor,SetCursorPos} needs WINSTA_WRITEATTRIBUTES
				if (show)
				{
					ClipCursor(0);
					SetCursorPos(
						cursor.pointPosition.x,
						cursor.pointPosition.y);
					::ShowCursor(TRUE);
				}
				else
				{
					ResetMouseState();
					cursor.pointPosition = Driver::GetRawCursorPosition();
					::ShowCursor(FALSE);
					math::Aabb<2, int> box(
						AabbPositionSize(
							GetWindow().GetPosition(),
							math::Vec2i(GetWindow().GetSize())));
					math::Vec2i center(Center(Shrink(box, 0, 1)));
					SetCursorPos(center.x, center.y);
					RECT rect(math::MakeRect(box));
					ClipCursor(&rect);
				}
				cursor.visible = show;
			}

			// system cursor state
			math::Vec2u Driver::GetRawCursorPosition() const
			{
				POINT pt;
				GetCursorPos(&pt);
				return math::MakeVector(pt);
			}
			math::Vec2i Driver::GetRawCursorTranslation() const
			{
				POINT pt;
				GetCursorPos(&pt);
				math::Aabb<2, int> box(
					AabbPositionSize(
						GetWindow().GetPosition(),
						math::Vec2i(GetWindow().GetSize())));
				math::Vec2i center(Center(Shrink(box, 0, 1)));
				SetCursorPos(center.x, center.y);
				return math::MakeVector(pt) - center;
			}
			bool Driver::GetMouseButton(Button button) const
			{
				if (GetSystemMetrics(SM_SWAPBUTTON))
					button = static_cast<Button>(std::abs(button - 2));
				int key;
				switch (button)
				{
					case leftButton:   key = VK_LBUTTON; break;
					case middleButton: key = VK_MBUTTON; break;
					case rightButton:  key = VK_RBUTTON; break;
					default: assert(!"invalid mouse button");
				}
				return GetAsyncKeyState(key) & 0x8000;
			}

			// cursor update
			void Driver::UpdateCursor()
			{
				if (GetWindow().HasFocus() &&
					ContainsMaxExclusive(
						AabbPositionSize(
							GetWindow().GetPosition(),
							math::Vec2i(GetWindow().GetSize())),
						GetRawCursorPosition()))
				{
					cursor.icon = Cursor::noIcon;
					PostMessage(GetWindow().GetHwnd(), WM_SETCURSOR,
						reinterpret_cast<WPARAM>(GetWindow().GetHwnd()),
						MAKELPARAM(HTCLIENT, WM_MOUSEMOVE));
				}
			}

			// inspiration notification
			void Driver::OnImbue(const ui::UserInterface *)
			{
				UpdateCursor();
			}

			// window signal handlers
			void Driver::OnFocus(bool focus)
			{
				if (GetCursorMode() == pointCursorMode)
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
							mouse.ignoreDown = true;
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
					limits = Limits();
					break;

					// cursor messages
					case WM_SETCURSOR:
					case WM_DISPLAYCHANGE:
					if (cursor.visible && LOWORD(lparam) == HTCLIENT)
					{
						Cursor::Icon targetIcon =
							GetWindow().HasFocus() && GetUserInterface() &&
							GetUserInterface()->GetTheme().cursor ?
							Cursor::themeIcon : Cursor::arrowIcon;
						HCURSOR handle;
						switch (targetIcon)
						{
							case Cursor::arrowIcon:
							handle = LoadCursor(NULL, IDC_ARROW);
							break;
							case Cursor::themeIcon:
							handle = *cache::win32::CursorProxy(
								GetUserInterface()->GetTheme().cursor,
								GetWindow().GetScreenSize().y);
							break;
							default: assert(!"invalid cursor icon");
						}
						SetCursor(handle);
						cursor.icon = targetIcon;
					}
					else cursor.icon = Cursor::noIcon;
					break;

					// keyboard messages
					case WM_CHAR:
					if (std::isprint(wparam)) charSig(wparam);
					break;
					case WM_KEYDOWN:
					if (wparam == VK_ESCAPE &&
						GetCursorMode() == pointCursorMode && mouse.down)
						ResetMouseState(NormScreenVector(math::MakeVector(GetMessagePos())));
					else
					{
						Key key;
						switch (wparam)
						{
							case VK_BACK:   key = backspaceKey; break;
							case VK_DELETE: key = deleteKey;    break;
							case VK_DOWN:   key = downKey;      break;
							case VK_RETURN: key = enterKey;     break;
							case VK_ESCAPE: key = escapeKey;    break;
							case VK_LEFT:   key = leftKey;      break;
							case VK_PAUSE:  key = pauseKey;     break;
							case VK_F12:    key = printKey;     break;
							case VK_F9:     key = recordKey;    break;
							case VK_RIGHT:  key = rightKey;     break;
							case VK_TAB:    key = tabKey;       break;
							case VK_UP:     key = upKey;        break;
							default: goto NoKey;
						}
						keySig(key);
						NoKey:;
					}
					break;

					// mouse messages
					case WM_LBUTTONDOWN: OnButtonDown(leftButton, wparam, lparam); break;
					case WM_MBUTTONDOWN: OnButtonDown(middleButton, wparam, lparam); break;
					case WM_RBUTTONDOWN: OnButtonDown(rightButton, wparam, lparam); break;
					case WM_LBUTTONUP: OnButtonUp(leftButton, wparam, lparam); break;
					case WM_MBUTTONUP: OnButtonUp(middleButton, wparam, lparam); break;
					case WM_RBUTTONUP: OnButtonUp(rightButton, wparam, lparam); break;
					case WM_MOUSEMOVE: OnMouseMove(wparam, lparam); break;
					case WM_MOUSEWHEEL: OnMouseWheel(wparam, lparam); break;
				}
				if (msg == mshMousewheel) OnMouseWheel(wparam, lparam);
			}
			void Driver::OnMove(const math::Vec2i &position)
			{
				if (GetCursorMode() == lookCursorMode)
				{
					// update cursor clipping
					ClipCursor(0);
					math::Aabb<2, int> box(
						AabbPositionSize(
							position,
							math::Vec2i(GetWindow().GetSize())));
					math::Vec2i center(Center(Shrink(box, 0, 1)));
					SetCursorPos(center.x, center.y);
					RECT rect(math::MakeRect(box));
					ClipCursor(&rect);
				}
			}
			void Driver::OnSize(const math::Vec2u &size)
			{
				if (GetCursorMode() == lookCursorMode)
				{
					// update cursor clipping
					ClipCursor(0);
					math::Aabb<2, int> box(
						AabbPositionSize(
							GetWindow().GetPosition(),
							math::Vec2i(size)));
					math::Vec2i center(Center(Shrink(box, 0, 1)));
					SetCursorPos(center.x, center.y);
					RECT rect(math::MakeRect(box));
					ClipCursor(&rect);
				}
			}

			// window message handlers
			void Driver::OnButtonDown(Button button, WPARAM wparam, LPARAM lparam)
			{
				if (GetCursorMode() != pointCursorMode) return;
				if (mouse.ignoreDown)
				{
					mouse.ignoreDown = false;
					return;
				}
				SetCapture(GetWindow().GetHwnd());
				StartMouseRepeatTimer();
				// cancel dragging
				math::Vec2u position(math::MakeVector(lparam));
				math::Vec2 normPosition(NormClientVector(position));
				ResetMouseDrag(normPosition);
				// signal down
				LONG time = GetMessageTime();
				mouse._double =
					button == mouse.downButton &&
					time - mouse.downTime <= limits.doubleTime &&
					All(Abs(
						math::Vec2i(position) -
						math::Vec2i(mouse.downPosition)) <= limits.doubleRange);
				downSig(normPosition, button, mouse._double);
				// update mouse state
				mouse.down = true;
				mouse.downButton = button;
				mouse.downTime = time;
				mouse.downPosition = position;
			}
			void Driver::OnButtonUp(Button button, WPARAM wparam, LPARAM lparam)
			{
				if (GetCursorMode() != pointCursorMode) return;
				if (mouse.down && button == mouse.downButton)
				{
					ReleaseCapture();
					StopMouseRepeatTimer();
					// signal click/drop
					math::Vec2u position(math::MakeVector(lparam));
					math::Vec2 normPosition(NormClientVector(position));
					if (mouse.dragging)
					{
						dropSig(
							NormClientVector(mouse.downPosition),
							normPosition, button, mouse._double);
						mouse.dragging = false;
						mouse.downTime = 0;
					}
					else clickSig(normPosition, button, mouse._double);
					// update mouse state
					mouse.down = false;
				}
			}
			void Driver::OnMouseMove(WPARAM wparam, LPARAM lparam)
			{
				if (GetCursorMode() != pointCursorMode) return;
				if (mouse.down && !mouse.dragging)
				{
					// check for dragging
					math::Vec2u position(math::MakeVector(lparam));
					if (Any(Abs(
							math::Vec2i(position) -
							math::Vec2i(mouse.downPosition)) >= limits.dragRange))
					{
						StopMouseRepeatTimer();
						dragSig(
							NormClientVector(mouse.downPosition),
							mouse.downButton, mouse._double);
						mouse.dragging = true;
					}
				}
			}
			void Driver::OnMouseWheel(WPARAM wparam, LPARAM lparam)
			{
				if (!wparam) return;
				float delta = static_cast<float>(GET_WHEEL_DELTA_WPARAM(wparam)) / WHEEL_DELTA;
				switch (GetCursorMode())
				{
					case pointCursorMode:
					scrollSig(NormScreenVector(math::win32::MakeVector(lparam)), delta);
					break;
					case lookCursorMode:
					mouse.deltaScroll += delta;
					break;
				}
			}

			// mouse state
			void Driver::ResetMouseState()
			{
				ResetMouseState(GetCursorPosition());
			}
			void Driver::ResetMouseState(const math::Vec2 &position)
			{
				assert(GetCursorMode() == pointCursorMode);
				if (mouse.dragging) ResetMouseDrag(position);
				else StopMouseRepeatTimer();
				mouse.down = false;
				mouse.downTime = 0;
			}
			void Driver::ResetMouseDrag(const math::Vec2 &position)
			{
				assert(GetCursorMode() == pointCursorMode);
				if (mouse.dragging)
				{
					cancelDragSig(
						NormClientVector(mouse.downPosition),
						position, mouse.downButton, mouse._double);
					mouse.dragging = false;
					mouse.downTime = 0;
				}
			}

			// mouse repeat timer
			void Driver::StartMouseRepeatTimer()
			{
				mouse.repeating = false;
				mouse.repeatTimer = SetTimer(0, mouse.repeatTimer, limits.repeatDelay, &Driver::MouseRepeatTimerRouter);
				GetMouseRepeatTimerDriverMap().insert(std::make_pair(mouse.repeatTimer, this));
			}
			void Driver::StopMouseRepeatTimer()
			{
				if (mouse.repeatTimer)
				{
					KillTimer(NULL, mouse.repeatTimer);
					GetMouseRepeatTimerDriverMap().erase(mouse.repeatTimer);
					mouse.repeatTimer = 0;
				}
			}

			// mouse repeat timer handling
			void Driver::OnMouseRepeatTimer()
			{
				if (!mouse.repeating)
				{
					mouse.repeating = true;
					SetTimer(0, mouse.repeatTimer, limits.repeatSpeed, &Driver::MouseRepeatTimerRouter);
				}
				downSig(
					NormScreenVector(math::win32::MakeVector(GetMessagePos())),
					mouse.downButton, mouse._double);
			}
			void CALLBACK Driver::MouseRepeatTimerRouter(HWND, UINT, UINT_PTR timer, DWORD)
			{
				MouseRepeatTimerDriverMap::const_iterator iter(GetMouseRepeatTimerDriverMap().find(timer));
				if (iter != GetMouseRepeatTimerDriverMap().end())
				{
					Driver &driver(*iter->second);
					driver.OnMouseRepeatTimer();
				}
			}

			// vector normalization
			math::Vec2 Driver::NormClientVector(const math::Vec2i &v) const
			{
				return Min(Max(EnterSpace(math::Aabb<2, int>(0, (GetWindow().GetSize() - 1)), v), 0), 1);
			}
			math::Vec2 Driver::NormScreenVector(const math::Vec2i &v) const
			{
				return NormClientVector(v - GetWindow().GetPosition());
			}

			// system limits
			// construct
			Driver::Limits::Limits()
			{
				doubleTime = GetDoubleClickTime();
				doubleRange = math::Vec2i(
					GetSystemMetrics(SM_CXDOUBLECLK),
					GetSystemMetrics(SM_CYDOUBLECLK)) / 2;
				dragRange = math::Vec2i(
					GetSystemMetrics(SM_CXDRAG),
					GetSystemMetrics(SM_CYDRAG)) / 2;
				if (!SystemParametersInfo(SPI_GETKEYBOARDDELAY, 0, &repeatDelay, 0))
					THROW((err::Exception<err::InpModuleTag, err::Win32PlatformTag>("failed to query keyboard repeat delay") <<
						boost::errinfo_api_function("SystemParametersInfo")))
				repeatDelay = (repeatDelay + 1) * 250;
				if (!SystemParametersInfo(SPI_GETKEYBOARDSPEED, 0, &repeatSpeed, 0))
					THROW((err::Exception<err::InpModuleTag, err::Win32PlatformTag>("failed to query keyboard repeat speed") <<
						boost::errinfo_api_function("SystemParametersInfo")))
				repeatSpeed = 400 - repeatSpeed * 12;
			}

			// cursor state
			// construct
			Driver::Cursor::Cursor() : icon(noIcon), visible(true) {}

			// mouse state
			// construct
			Driver::Mouse::Mouse() :
				down(false), dragging(false), _double(false), ignoreDown(false),
				downTime(0), repeating(false), repeatTimer(0) {}
		}

		// factory function
		Driver *MakeDriver(wnd::Window &wnd)
		{
			return new win32::Driver(dynamic_cast<wnd::win32::Window &>(wnd));
		}
	}
}
