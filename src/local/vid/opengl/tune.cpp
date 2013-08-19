#include <iostream> // cout
#include <string>

#include <GL/gl.h> // glGetString

#include "../../cfg/domain.hpp" // ScopedDomain
#include "../../cfg/vars.hpp"
#include "../../log/Indenter.hpp"
#include "ext.hpp" // ARB_{fragment_shader,shader_objects,shadow,vertex_buffer_object}, EXT_framebuffer_object, NV_register_combiners

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			void Tune()
			{
				cfg::ScopedDomain domain(cfg::siteDomain);
				if (haveExtFramebufferObject)
				{
					// FIXME: implement
				}
				if (haveArbVertexBufferObject)
				{
					// profile vertex arrays
					// FIXME: implement
					float varTime = 1;
					// profile vertex buffers
					// FIXME: implement
					float vboTime = 1;
					// determine optimal solution
					cfg::opengl::renderVbo = vboTime <= varTime;
				}
				std::cout << "vertex buffering = " << (*CVAR(opengl)::renderVbo ? "enabled" : "disabled") << std::endl;
				if (haveExtFramebufferObject && haveArbShadow)
				{
					// FIXME: implement
					cfg::opengl::renderShadow = true;
				}
				std::cout << "shadowing        = " << (*CVAR(opengl)::renderBump ? "enabled" : "disabled") << std::endl;
				if (haveArbFragmentShader && haveArbShaderObjects ||
					haveNvRegisterCombiners)
				{
					// FIXME: implement
					cfg::opengl::renderBump = true;
				}
				std::cout << "bump mapping     = " << (*CVAR(opengl)::renderBump ? "enabled" : "disabled") << std::endl;
			}
			void InitProfile()
			{
				if (!*CVAR(opengl)::renderTuneAuto) return;
				cfg::ScopedDomain domain(cfg::siteDomain);
				std::string
					deviceProfile(std::string(reinterpret_cast<const char *>(glGetString(GL_VENDOR))) + ' ' + reinterpret_cast<const char *>(glGetString(GL_RENDERER))),
					profile(deviceProfile + ' ' + reinterpret_cast<const char *>(glGetString(GL_VERSION)));
				if (profile != *CVAR(opengl)::renderTuneProfile)
				{
					std::cout << "tuning new " <<
						(deviceProfile == cfg::opengl::renderTuneProfile->substr(0, deviceProfile.size()) ?
						"driver" : "device") << std::endl;
					log::Indenter indenter;
					Tune();
					cfg::opengl::renderTuneProfile = profile;
				}
			}
		}
	}
}
