#include <algorithm> // max
#include <array>
#include <iomanip> // setw
#include <iostream> // cout
#include <iterator> // inserter
#include <string>
#include <unordered_set>
#include <vector>

#include <GL/glx.h>
#include <GL/glxext.h>

#include <boost/io/ios_state.hpp> // ios_all_saver

#include "../../../cfg/vars.hpp"
#include "../../../log/Indenter.hpp"
#include "../../../util/cpp.hpp" // STRINGIZE
#include "../../../util/io/deserialize.hpp" // Deserialize
#include "../ext.hpp" // GetProcAddress, ProcAddress

// HACK: import glXGetProcAddress
void (*glXGetProcAddress(const GLubyte *))();

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			namespace x11
			{
				bool
					haveArbGetProcAddress,
					haveExtVisualInfo;

				// ARB_get_proc_address
				PFNGLXGETPROCADDRESSARBPROC glXGetProcAddressARB;
				// EXT_visual_info

				namespace
				{
#define PROC_ADDRESS(a) reinterpret_cast<ProcAddress *>(&a)
#define PROC(a) STRINGIZE(a), PROC_ADDRESS(a)

					struct Proc
					{
						std::string name;
						ProcAddress *address;
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
					typedef std::array<Ext, 1> Exts;
					Exts exts =
					{
						"GLX_EXT_visual_info", haveExtVisualInfo, NO_PROCS
					};
				}

				// function binding initialization
				void InitExt(Display *display, int screen)
				{
					// check for GLX_ARB_get_proc_address
					// FIXME: implement using glXQueryExtensionsString
					glXGetProcAddressARB = glXGetProcAddress;
					haveArbGetProcAddress = true;
					std::cout << "loading X11 OpenGL extensions" << std::endl;
					log::Indenter indenter;

					// build supported extension set
					std::string extString(glXQueryExtensionsString(display, screen));
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
				return reinterpret_cast<ProcAddress>(glXGetProcAddress(
					reinterpret_cast<const GLubyte *>(name.c_str())));
			}
		}
	}
}
