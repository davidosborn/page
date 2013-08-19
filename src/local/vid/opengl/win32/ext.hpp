#ifndef    page_local_vid_opengl_win32_ext_hpp
#   define page_local_vid_opengl_win32_ext_hpp

#	include <GL/gl.h>
#	include <GL/wglext.h>

#	include <windows.h> // HDC

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			namespace win32
			{
				extern bool
					haveArbExtensionsString,
					haveArbMakeCurrentRead,
					haveArbPbuffer,
					haveArbPixelFormat,
					haveArbRenderTexture,
					haveExtSwapControl;

				// ARB_extensions_string
				extern PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB;
				// ARB_make_current_read
				extern PFNWGLMAKECONTEXTCURRENTARBPROC wglMakeContextCurrentARB;
				extern PFNWGLGETCURRENTREADDCARBPROC wglGetCurrentReadDCARB;
				// ARB_pbuffer
				extern PFNWGLCREATEPBUFFERARBPROC wglCreatePbufferARB;
				extern PFNWGLGETPBUFFERDCARBPROC wglGetPbufferDCARB;
				extern PFNWGLRELEASEPBUFFERDCARBPROC wglReleasePbufferDCARB;
				extern PFNWGLDESTROYPBUFFERARBPROC wglDestroyPbufferARB;
				extern PFNWGLQUERYPBUFFERARBPROC wglQueryPbufferARB;
				// ARB_pixel_format
				extern PFNWGLGETPIXELFORMATATTRIBIVARBPROC wglGetPixelFormatAttribivARB;
				extern PFNWGLGETPIXELFORMATATTRIBFVARBPROC wglGetPixelFormatAttribfvARB;
				extern PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
				// ARB_render_texture
				extern PFNWGLBINDTEXIMAGEARBPROC wglBindTexImageARB;
				extern PFNWGLRELEASETEXIMAGEARBPROC wglReleaseTexImageARB;
				extern PFNWGLSETPBUFFERATTRIBARBPROC wglSetPbufferAttribARB;
				// EXT_swap_control
				extern PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;
				extern PFNWGLGETSWAPINTERVALEXTPROC wglGetSwapIntervalEXT;

				// function binding initialization
				// must be called before using extensions
				void InitExt(HDC);
			}
		}
	}
}

#endif
