#ifndef    page_local_wnd_win32_Window_hpp
#   define page_local_wnd_win32_Window_hpp

#	include <string>

#	include <windows.h> // HWND, LPARAM, LRESULT, UINT, WPARAM

#	include "../Window.hpp"

namespace page { namespace wnd { namespace win32
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
		HWND GetHwnd() const;

		// message signal
		boost::signal<void (UINT, WPARAM, LPARAM)> messageSig;

		// environment state
		math::Vec2u GetScreenSize() const;

		private:
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
