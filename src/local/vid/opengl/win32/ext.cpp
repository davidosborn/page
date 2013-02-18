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

#include <algorithm> // max
#include <array>
#include <iomanip> // setw
#include <iostream> // cout
#include <iterator> // inserter
#include <string>
#include <unordered_set>
#include <vector>
#include <windows.h> // HDC, wglGetProcAddress
#include <GL/gl.h>
#include <GL/wglext.h>

#include <boost/io/ios_state.hpp> // ios_all_saver

#include "../../../cfg/vars.hpp"
#include "../../../log/Indenter.hpp"
#include "../../../util/pp.hpp" // STRINGIZE
#include "../../../util/serialize/deserialize_string.hpp" // Deserialize
#include "../ext.hpp" // GetProcAddress, ProcAddress

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			namespace win32
			{
				bool
					haveArbExtensionsString,
					haveArbMakeCurrentRead,
					haveArbPbuffer,
					haveArbPixelFormat,
					haveArbRenderTexture,
					haveExtSwapControl;

				// ARB_extensions_string
				PFNWGLGETEXTENSIONSSTRINGARBPROC    wglGetExtensionsStringARB;
				// ARB_make_current_read
				PFNWGLMAKECONTEXTCURRENTARBPROC     wglMakeContextCurrentARB;
				PFNWGLGETCURRENTREADDCARBPROC       wglGetCurrentReadDCARB;
				// ARB_pbuffer
				PFNWGLCREATEPBUFFERARBPROC          wglCreatePbufferARB;
				PFNWGLGETPBUFFERDCARBPROC           wglGetPbufferDCARB;
				PFNWGLRELEASEPBUFFERDCARBPROC       wglReleasePbufferDCARB;
				PFNWGLDESTROYPBUFFERARBPROC         wglDestroyPbufferARB;
				PFNWGLQUERYPBUFFERARBPROC           wglQueryPbufferARB;
				// ARB_pixel_format
				PFNWGLGETPIXELFORMATATTRIBIVARBPROC wglGetPixelFormatAttribivARB;
				PFNWGLGETPIXELFORMATATTRIBFVARBPROC wglGetPixelFormatAttribfvARB;
				PFNWGLCHOOSEPIXELFORMATARBPROC      wglChoosePixelFormatARB;
				// ARB_render_texture
				PFNWGLBINDTEXIMAGEARBPROC           wglBindTexImageARB;
				PFNWGLRELEASETEXIMAGEARBPROC        wglReleaseTexImageARB;
				PFNWGLSETPBUFFERATTRIBARBPROC       wglSetPbufferAttribARB;
				// EXT_swap_control
				PFNWGLSWAPINTERVALEXTPROC           wglSwapIntervalEXT;
				PFNWGLGETSWAPINTERVALEXTPROC        wglGetSwapIntervalEXT;

				namespace
				{
#define PROC_ADDRESS(a) reinterpret_cast<ProcAddress *>(&a)
#define PROC(a) STRINGIZE(a), PROC_ADDRESS(a)

					struct Proc
					{
						std::string name;
						ProcAddress *address;
					};
					const std::array<Proc, 2> arbMakeCurrentReadProcs =
					{
						PROC(wglMakeContextCurrentARB),
						PROC(wglGetCurrentReadDCARB)
					};
					const std::array<Proc, 5> arbPbufferProcs =
					{
						PROC(wglCreatePbufferARB),
						PROC(wglGetPbufferDCARB),
						PROC(wglReleasePbufferDCARB),
						PROC(wglDestroyPbufferARB),
						PROC(wglQueryPbufferARB)
					};
					const std::array<Proc, 3> arbPixelFormatProcs =
					{
						PROC(wglGetPixelFormatAttribivARB),
						PROC(wglGetPixelFormatAttribfvARB),
						PROC(wglChoosePixelFormatARB)
					};
					const std::array<Proc, 3> arbRenderTextureProcs =
					{
						PROC(wglBindTexImageARB),
						PROC(wglReleaseTexImageARB),
						PROC(wglSetPbufferAttribARB)
					};
					const std::array<Proc, 2> extSwapControlProcs =
					{
						PROC(wglSwapIntervalEXT),
						PROC(wglGetSwapIntervalEXT)
					};

#define PROCS(a) Ext::Procs(a.begin(), a.end())
#define NO_PROCS Ext::Procs()

					struct Ext
					{
						const std::string name;
						bool &have;
						typedef std::vector<Proc> Procs;
						Procs procs;
					};
					typedef std::array<Ext, 5> Exts;
					Exts exts =
					{
						"WGL_ARB_make_current_read", haveArbMakeCurrentRead, PROCS(arbMakeCurrentReadProcs),
						"WGL_ARB_pbuffer",           haveArbPbuffer,         PROCS(arbPbufferProcs),
						"WGL_ARB_pixel_format",      haveArbPixelFormat,     PROCS(arbPixelFormatProcs),
						"WGL_ARB_render_texture",    haveArbRenderTexture,   PROCS(arbRenderTextureProcs),
						"WGL_EXT_swap_control",      haveExtSwapControl,     PROCS(extSwapControlProcs)
					};
				}

				// function binding initialization
				void InitExt(HDC hdc)
				{
					// check for WGL_ARB_extensions_string
					if (!(wglGetExtensionsStringARB =
						reinterpret_cast<PFNWGLGETEXTENSIONSSTRINGARBPROC>(
						GetProcAddress("wglGetExtensionsStringARB")))) return;
					haveArbExtensionsString = true;
					std::cout << "loading Win32 OpenGL extensions" << std::endl;
					log::Indenter indenter;

					// build supported extension set
					std::string extString(reinterpret_cast<const char *>(wglGetExtensionsStringARB(hdc)));
					std::unordered_set<std::string> supportedExts;
					util::Deserialize(extString, std::inserter(supportedExts, supportedExts.end()));

					// calculate extension alignment width
					std::streamsize width = 0;
					for (Exts::const_iterator ext(exts.begin()); ext != exts.end(); ++ext)
						width = std::max(static_cast<std::streamsize>(ext->name.size()), width);

					// set flags for trailing alignment
					boost::io::ios_all_saver iosFormatSaver(std::cout);
					std::cout.setf(std::ios_base::left, std::ios_base::adjustfield);
					std::cout.fill(' ');

					// check relevant extensions
					for (Exts::iterator ext(exts.begin()); ext != exts.end(); ++ext)
					{
						std::cout << std::setw(width) << ext->name << " = ";
						bool broken = false;
						if (ext->have = supportedExts.find(ext->name) != supportedExts.end())
						{
							// initialize function pointers
							for (Ext::Procs::const_iterator proc(ext->procs.begin()); proc != ext->procs.end(); ++proc)
							{
								if (!(*proc->address = GetProcAddress(proc->name)))
								{
									if (!broken)
									{
										std::cout << "broken" << std::endl;
										broken = true;
										ext->have = false;
									}
									if (*CVAR(logVerbose))
									{
										log::Indenter indenter;
										std::cout << "missing function " << proc->name << std::endl;
									}
								}
							}
						}
						else
						{
							// reset function pointers
							for (Ext::Procs::const_iterator proc(ext->procs.begin()); proc != ext->procs.end(); ++proc)
								*proc->address = 0;
						}
						if (!broken) std::cout << (ext->have ? "good" : "missing") << std::endl;
					}
				}
			}

			// function address resolution
			ProcAddress GetProcAddress(const std::string &name)
			{
				return reinterpret_cast<ProcAddress>(wglGetProcAddress(name.c_str()));
			}
		}
	}
}
