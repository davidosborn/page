#ifndef    page_local_wnd_x11_Window_hpp
#   define page_local_wnd_x11_Window_hpp

#	include <X11/Xlib.h> // Atom, Display, Window, XEvent

#	include "../Window.hpp"

namespace page { namespace wnd { namespace x11
{
	class Window : public wnd::Window
	{
		IMPLEMENT_CLONEABLE(Window, wnd::Window)

		public:
		// construct/destroy
		explicit Window(const std::string &title);
		~Window();

		// update
		void Update();

		// platform access
		Display *GetDisplay() const;
		int GetScreen() const;
		::Window GetWindow() const;

		// event signal
		util::copyable_signal<void (const XEvent &)> eventSig;

		// environment state
		math::Vec2u GetScreenSize() const;

		private:
		// driver factory functions
		aud::Driver *MakeAudioDriver();
		inp::Driver *MakeInputDriver();
		vid::Driver *MakeVideoDriver();

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
