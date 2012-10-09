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

#ifndef    page_vid_opengl_win32_ext_hpp
#   define page_vid_opengl_win32_ext_hpp

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
