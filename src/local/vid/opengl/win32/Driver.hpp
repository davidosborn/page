#ifndef    page_local_vid_opengl_win32_Driver_hpp
#   define page_local_vid_opengl_win32_Driver_hpp

#	include "../Driver.hpp"

namespace page
{
	namespace wnd { namespace win32 { class Window; }}

	namespace vid
	{
		namespace opengl
		{
			namespace win32
			{
				struct Driver : opengl::Driver
				{
					// construct
					explicit Driver(wnd::win32::Window &);
					~Driver();

					private:
					// extension initialization
					void InitAdapterExt();

					// framebuffer modifiers
					void Flush();

					HWND hwnd;
					HDC hdc;
					HGLRC hglrc;
				};
			}
		}
	}
}

#endif
