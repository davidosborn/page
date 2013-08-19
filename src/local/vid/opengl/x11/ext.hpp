#ifndef    page_local_vid_opengl_x11_ext_hpp
#   define page_local_vid_opengl_x11_ext_hpp

#	include <GL/glx.h>
#	include <GL/glxext.h>

#	include <X11/Xlib.h> // Display

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			namespace x11
			{
				extern bool
					haveArbGetProcAddress,
					haveExtVisualInfo;

				// ARB_get_proc_address
				extern PFNGLXGETPROCADDRESSARBPROC glxGetProcAddressARB;
				// EXT_visual_info

				// function binding initialization
				// must be called before using extensions
				void InitExt(Display *, int screen);
			}
		}
	}
}

#endif
