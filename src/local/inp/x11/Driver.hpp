#ifndef    page_local_inp_x11_Driver_hpp
#   define page_local_inp_x11_Driver_hpp

#	include <X11/Xlib.h> // KeySym, Time, XEvent

#	include "../Driver.hpp"

namespace page
{
	namespace inp
	{
		namespace x11
		{
			struct Driver : inp::Driver
			{
				// construct
				explicit Driver(wnd::x11::Window &);

				// window access
				wnd::x11::Window &GetWindow();
				const wnd::x11::Window &GetWindow() const;

				private:
				// state query
				State Poll() const;

				// cursor mode modifiers
				void DoSetCursorMode(CursorMode);

				// system cursor state
				math::Vec2u GetRawCursorPosition() const;

				// window signal handlers
				void OnEvent(const XEvent &);
				boost::signals::scoped_connection eventCon;

				// key state
				bool IsKeyDown(KeySym, const char keys[32]) const;

				// cursor state
				struct
				{
					math::Vec2u pointPosition;
				} cursor;
			};
		}
	}
}

#endif
