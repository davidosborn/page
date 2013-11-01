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
