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

#include <unordered_map>

#include <windowsx.h> // GET_[XY]_LPARAM

#include "../../aud/Driver.hpp" // MakeDriver
#include "../../cfg/vars.hpp"
#include "../../err/Exception.hpp"
#include "../../inp/Driver.hpp" // MakeDriver
#include "../../res/type/image/win32.hpp" // MakeBitmap
#include "../../util/pp.hpp" // STRINGIZE
#include "../../util/win32/string.hpp" // Native
#include "../../vid/Driver.hpp" // Driver::RenderImage, MakeDriver
#include "../../win32/resource.h" // IDI_ICON
#include "Window.hpp"

namespace page
{
	namespace wnd
	{
		namespace win32
		{
			namespace
			{
				// window handle mapping
				typedef std::unordered_map<HWND, Window *> WindowMap;
				inline WindowMap &GetWindowMap()
				{
					static WindowMap map;
					return map;
				}
				Window *lastWindow;

				// critical section for thread-safe window handle mapping
				// initialization
				struct CriticalSection
				{
					CriticalSection();
					~CriticalSection();

					CRITICAL_SECTION cs;
				};
				CriticalSection::CriticalSection()
				{
					InitializeCriticalSection(&cs);
				}
				CriticalSection::~CriticalSection()
				{
					DeleteCriticalSection(&cs);
				}
				inline CRITICAL_SECTION &GetCriticalSection()
				{
					static CriticalSection cs;
					return cs.cs;
				}
			}

			// construct/destroy
			Window::Window(const std::string &title) :
				alive(false), min(false), full(*CVAR(windowFullscreen)),
				waitFocus(false), moving(false), sizing(false),
				muteFocus(false), muteMove(false), muteSize(false),
				clientBitmap(NULL)
			{
				DWORD style;
				RECT rect;
				if (full)
				{
					try
					{
						SetFull(true);
					}
					catch (const err::Exception<err::Win32PlatformTag>::Permutation &)
					{
						CVAR(windowFullscreen) = full = false;
						goto InitWindowed;
					}
					style = WS_POPUP;
					rect.left = rect.top = 0;
					rect.right  = CVAR(videoResolution)->x;
					rect.bottom = CVAR(videoResolution)->y;
				}
				else
				{
					InitWindowed:
					style = WS_OVERLAPPEDWINDOW;
					rect.right  = (rect.left = CVAR(windowPosition)->x) + CVAR(windowSize)->x;
					rect.bottom = (rect.top  = CVAR(windowPosition)->y) + CVAR(windowSize)->y;
					AdjustWindowRectEx(&rect, style, FALSE, WS_EX_APPWINDOW);
				}
				// create window
				EnterCriticalSection(&GetCriticalSection());
				lastWindow = this;
				if (!(hwnd = CreateWindowEx(WS_EX_APPWINDOW, GetClass(),
					util::win32::Native(title).c_str(), style,
					rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
					0, 0, reinterpret_cast<HINSTANCE>(GetModuleHandle(0)), 0)))
				{
					LeaveCriticalSection(&GetCriticalSection());
					if (full) SetFull(false);
					THROW((err::Exception<err::EnvModuleTag, err::Win32PlatformTag>("failed to create window") <<
						boost::errinfo_api_function("CreateWindowEx")))
				}
				GetWindowMap().insert(std::make_pair(hwnd, this));
				LeaveCriticalSection(&GetCriticalSection());
				// show window
				SetForegroundWindow(hwnd);
				ShowWindow(hwnd, *CVAR(windowMaximized) ? SW_SHOWMAXIMIZED : SW_SHOWNORMAL);
				if (GetForegroundWindow() != hwnd) waitFocus = true;
				// process immediate messages
				MSG msg;
				while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				alive = true;
				// set initial state
				GetClientRect(hwnd, &rect);
				MapWindowPoints(hwnd, 0, reinterpret_cast<LPPOINT>(&rect), 1);
				InitState(GetForegroundWindow() == hwnd,
					math::Vector<2, int>(rect.left, rect.top),
					math::Vector<2, unsigned>(rect.right, rect.bottom));
			}
			Window::~Window()
			{
				Deinit();
				alive = false;
				if (clientBitmap) DeleteObject(clientBitmap);
				DestroyWindow(hwnd);
				GetWindowMap().erase(hwnd);
				if (full) SetFull(false);
			}

			// update
			void Window::Update()
			{
				MSG msg;
				while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}

			// platform access
			HWND Window::GetHwnd() const
			{
				return hwnd;
			}

			// environment state
			math::Vector<2, unsigned> Window::GetScreenSize() const
			{
				// FIXME: it would be best to use the device context of the
				// screen that contains the window
				HDC hdc = GetDC(NULL);
				if (!hdc)
					THROW((err::Exception<err::EnvModuleTag, err::Win32PlatformTag>("failed to get device context") <<
						boost::errinfo_api_function("GetDC")))
				math::Vector<2, unsigned> size(
					GetDeviceCaps(hdc, HORZRES),
					GetDeviceCaps(hdc, VERTRES));
				ReleaseDC(NULL, hdc);
				return size;
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
				if (full)
				{
					DEVMODE dm = {};
					dm.dmSize       = sizeof dm;
					dm.dmFields     = DM_PELSWIDTH | DM_PELSHEIGHT;
					dm.dmPelsWidth  = CVAR(videoResolution)->x;
					dm.dmPelsHeight = CVAR(videoResolution)->y;

					// maintain refresh rate
					if (HDC hdc = GetDC(NULL))
					{
						if (GetDeviceCaps(hdc, HORZRES) == CVAR(videoResolution)->x &&
							GetDeviceCaps(hdc, VERTRES) == CVAR(videoResolution)->y)
						{
							dm.dmFields |= DM_DISPLAYFREQUENCY;
							dm.dmDisplayFrequency = GetDeviceCaps(hdc, VREFRESH);
						}
						ReleaseDC(NULL, hdc);
					}
					if (ChangeDisplaySettings(&dm, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
						THROW((err::Exception<err::EnvModuleTag, err::Win32PlatformTag>("failed to change display mode") <<
							boost::errinfo_api_function("ChangeDisplaySettings")))
				}
				else if (ChangeDisplaySettings(0, 0) != DISP_CHANGE_SUCCESSFUL)
					THROW((err::Exception<err::EnvModuleTag, err::Win32PlatformTag>("failed to change display mode") <<
						boost::errinfo_api_function("ChangeDisplaySettings")))
			}
			void Window::SwitchFull()
			{
				muteFocus = muteMove = muteSize = true;
				if (full = !full)
				{
					try
					{
						SetFull(true);
					}
					catch (...)
					{
						full = false;
						muteFocus = muteMove = muteSize = false;
						throw;
					}
					SetWindowLongPtr(hwnd, GWL_STYLE, WS_POPUP);
					muteMove = muteSize = false;
					SetWindowPos(hwnd, 0, 0, 0, CVAR(videoResolution)->x, CVAR(videoResolution)->y, SWP_SHOWWINDOW | SWP_NOCOPYBITS | SWP_NOOWNERZORDER | SWP_NOZORDER);
					SetForegroundWindow(hwnd);
				}
				else
				{
					ShowWindow(hwnd, SW_HIDE);
					try
					{
						SetFull(false);
					}
					catch (...)
					{
						full = true;
						muteFocus = muteMove = muteSize = false;
						throw;
					}
					RECT rect;
					rect.right  = (rect.left = CVAR(windowPosition)->x) + CVAR(windowSize)->x;
					rect.bottom = (rect.top  = CVAR(windowPosition)->y) + CVAR(windowSize)->y;
					AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, false, WS_EX_APPWINDOW);
					SetWindowLongPtr(hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
					muteMove = muteSize = false;
					SetWindowPos(hwnd, 0, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER | SWP_NOZORDER);
					ShowWindow(hwnd, *CVAR(windowMaximized) ? SW_SHOWMAXIMIZED : SW_SHOW);
					SetForegroundWindow(hwnd);
				}
				muteFocus = muteMove = muteSize = false;
				CVAR(windowFullscreen) = full;
			}

			// message handling
			LRESULT Window::WindowProc(UINT msg, WPARAM wparam, LPARAM lparam)
			{
				switch (msg)
				{
					case WM_CLOSE:
					exitSig();
					return 0;

					case WM_SYSCOMMAND:
					switch (wparam)
					{
						case SC_MINIMIZE:
						case SC_MINIMIZE + 2:
						min = true;
						break;
						case SC_MAXIMIZE:
						case SC_MAXIMIZE + 2:
						CVAR(windowMaximized) = true;
						break;
						case SC_RESTORE:
						case SC_RESTORE + 2:
						if (!min) CVAR(windowMaximized) = false;
						break;
					}
					break;

					case WM_KEYDOWN:
					switch (wparam)
					{
						case VK_F11:
						SwitchFull();
						break;
					}
					break;
					case WM_SYSKEYDOWN:
					switch (wparam)
					{
						case VK_RETURN:
						SwitchFull();
						break;
					}
					break;

					case WM_WINDOWPOSCHANGED:
					if (!waitFocus) break;
					waitFocus = false;
					case WM_SETFOCUS:
					min = false;
					if (alive && !muteFocus) focusSig(true);
					break;
					case WM_KILLFOCUS:
					if (alive && !muteFocus)
					{
						focusSig(false);
						// save client area
						if (HasVideoDriver())
							clientBitmap = res::win32::MakeBitmap(
								GetVideoDriver().RenderImage(GetSize()));
					}
					break;

					case WM_MOVING:
					moving = true;
					break;
					case WM_MOVE:
					if (sizing) moving = true; // HACK: top-left frame sizing
					if (alive && !min && !muteMove && !moving && wparam != 0x83008300)
					{
						math::Vector<2, int> newPos(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
						if (!*CVAR(windowMaximized) && !full) CVAR(windowPosition) = newPos;
						moveSig(newPos);
					}
					break;
					case WM_SIZING:
					sizing = true;
					break;
					case WM_SIZE:
					if (alive && !min && !muteSize && !sizing && wparam != SIZE_MINIMIZED)
					{
						math::Vector<2, unsigned> newSize(LOWORD(lparam), HIWORD(lparam));
						if (!*CVAR(windowMaximized) && !full) CVAR(windowSize) = newSize;
						sizeSig(newSize);
					}
					break;
					case WM_EXITSIZEMOVE:
					if (alive && !min)
					{
						if (moving)
						{
							RECT rect;
							GetClientRect(hwnd, &rect);
							MapWindowPoints(hwnd, 0, reinterpret_cast<LPPOINT>(&rect), 1);
							moveSig(*(CVAR(windowPosition) = math::Vector<2, int>(rect.left, rect.top)));
							moving = false;
						}
						if (sizing)
						{
							RECT rect;
							GetClientRect(hwnd, &rect);
							sizeSig(*(CVAR(windowSize) = math::Vector<2, unsigned>(rect.right, rect.bottom)));
							sizing = false;
						}
					}
					break;

					case WM_PAINT:
					if (alive && !HasFocus() && clientBitmap)
					{
						RECT rect;
						if (GetUpdateRect(hwnd, &rect, false))
						{
							PAINTSTRUCT ps;
							if (BeginPaint(hwnd, &ps))
							{
								HDC hbmdc = CreateCompatibleDC(ps.hdc);
								HGDIOBJ prevObj = SelectObject(hbmdc, clientBitmap);
								BitBlt(
									ps.hdc,
									ps.rcPaint.left, ps.rcPaint.top,
									ps.rcPaint.right - ps.rcPaint.left,
									ps.rcPaint.bottom - ps.rcPaint.top,
									hbmdc,
									ps.rcPaint.left, ps.rcPaint.top,
									SRCCOPY);
								SelectObject(hbmdc, prevObj);
								DeleteDC(hbmdc);
								EndPaint(hwnd, &ps);
							}
						}
					}
					break;
				}
				return DefWindowProc(hwnd, msg, wparam, lparam);
			}
			LRESULT CALLBACK Window::MsgRouter(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
			{
				WindowMap::const_iterator iter(GetWindowMap().find(hwnd));
				Window *wnd;
				if (iter != GetWindowMap().end())
					wnd = iter->second;
				else
				{
					wnd = lastWindow;
					wnd->hwnd = hwnd;
				}
				wnd->messageSig(msg, wparam, lparam);
				return wnd->WindowProc(msg, wparam, lparam);
			}

			// window class
			LPCTSTR Window::MakeClass()
			{
				WNDCLASSEX wc = {sizeof wc};
				wc.lpfnWndProc = &Window::MsgRouter;
				wc.hInstance = reinterpret_cast<HINSTANCE>(GetModuleHandle(0));
				wc.hIcon = LoadIcon(wc.hInstance, MAKEINTRESOURCE(IDI_ICON));
				wc.lpszClassName = TEXT(STRINGIZE(NAME));
				ATOM atom = RegisterClassEx(&wc);
				if (!atom)
					THROW((err::Exception<err::EnvModuleTag, err::Win32PlatformTag>("failed to register window class") <<
						boost::errinfo_api_function("RegisterClassEx")))
				return reinterpret_cast<LPTSTR>(MAKEINTATOM(atom));
			}
			LPCTSTR Window::GetClass()
			{
				static LPCTSTR name = MakeClass();
				return name;
			}
		}

		// factory function
		Window *MakeWindow(const std::string &title)
			{ return new win32::Window(title); }
	}
}
