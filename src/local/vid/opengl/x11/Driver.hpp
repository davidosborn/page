#ifndef    page_local_vid_opengl_x11_Driver_hpp
#   define page_local_vid_opengl_x11_Driver_hpp

#	include <GL/glx.h> // GLXContext

#	include "../Driver.hpp"

namespace page
{
	namespace wnd { namespace x11 { class Window; }}

	namespace vid
	{
		namespace opengl
		{
			namespace x11
			{
				struct Driver : opengl::Driver
				{
					// construct
					explicit Driver(wnd::x11::Window &);
					~Driver();

					// window access
					wnd::x11::Window &GetWindow();
					const wnd::x11::Window &GetWindow() const;

					private:
					// extension initialization
					void InitAdapterExt();

					// framebuffer modifiers
					void Flush();

					GLXContext context;
				};
			}
		}
	}
}

#endif
