#include <iostream> // cout

#include <boost/optional.hpp>

#include "../../../cache/proxy/ResourceProxy.hpp"
#include "../../../cfg/vars.hpp"
#include "../../../cfg/vars.hpp"
#include "../../../err/report.hpp" // ReportWarning, std::exception
#include "../../../log/Indenter.hpp"
#include "../../../math/Vector.hpp"
#include "../ext.hpp" // ARB_{{fragment,vertex}_shader,shader_objects}, EXT_framebuffer_object
#include "../Program.hpp"
#include "../RenderTargetPool.hpp"
#include "ShaderOutlineResources.hpp"

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			const unsigned renderTargetSize = 768;

			// construct
			ShaderOutlineResources::ShaderOutlineResources(const math::Vec2u &size)
			{
				// create program
				{
					boost::optional<log::Indenter> indenter;
					if (*CVAR(logVerbose))
					{
						std::cout << "creating program" << std::endl;
						indenter = boost::in_place();
					}
					try
					{
						program.reset(new Program(
							*cache::ResourceProxy<res::opengl::Shader>("shader/glsl/outline.fs"),
							*cache::ResourceProxy<res::opengl::Shader>("shader/glsl/outline.vs")));
						program->GetUniform("texSize");
					}
					catch (const std::exception &e)
					{
						err::ReportWarning(e);
						throw;
					}
				}
				// create render-target pool
				{
					boost::optional<log::Indenter> indenter;
					if (*CVAR(logVerbose))
					{
						std::cout << "creating render-target pool" << std::endl;
						indenter = boost::in_place();
					}
					try
					{
						renderTargetPool.reset(new RenderTargetPool(
							Ceil(renderTargetSize * math::Vec2(Aspect(size), 1)),
							GL_RGB, 1, depthFramebufferFlag));
					}
					catch (const std::exception &e)
					{
						err::ReportWarning(e);
						throw;
					}
				}
			}

			// access
			const Program &ShaderOutlineResources::GetProgram() const
			{
				return *program;
			}
			const RenderTargetPool &ShaderOutlineResources::GetRenderTargetPool() const
			{
				return *renderTargetPool;
			}

			// compatibility
			bool ShaderOutlineResources::Check()
			{
				return
					*CVAR(opengl)::renderOutline &&
					*CVAR(opengl)::renderShader &&
					haveArbFragmentShader &&
					haveArbShaderObjects &&
					haveArbVertexShader &&
					haveExtFramebufferObject;
			}
		}
	}
}
