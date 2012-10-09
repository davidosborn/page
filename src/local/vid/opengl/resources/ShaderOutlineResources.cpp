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

#include <iostream> // cout
#include "../../../cache/proxy/Resource.hpp"
#include "../../../cfg.hpp" // logVerbose
#include "../../../cfg/opengl.hpp" // vid{Outline,Shader}
#include "../../../err/exception/catch.hpp" // CATCH_ALL_AND_PRINT_WARNING_AND
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
			ShaderOutlineResources::ShaderOutlineResources(const math::Vector<2, unsigned> &size)
			{
				// create program
				{
					log::Indenter indenter(false);
					if (*cfg::logVerbose)
					{
						std::cout << "creating program" << std::endl;
						indenter.Reset(true);
					}
					try
					{
						program.reset(new Program(
							*cache::Resource<res::opengl::Shader>("shader/glsl/outline.fs"),
							*cache::Resource<res::opengl::Shader>("shader/glsl/outline.vs")));
						program->GetUniform("texSize");
					}
					CATCH_ALL_AND_PRINT_WARNING_AND(throw;)
				}
				// create render-target pool
				{
					log::Indenter indenter(false);
					if (*cfg::logVerbose)
					{
						std::cout << "creating render-target pool" << std::endl;
						indenter.Reset(true);
					}
					try
					{
						renderTargetPool.reset(new RenderTargetPool(
							Ceil(renderTargetSize * math::Vector<2>(Aspect(size), 1)),
							GL_RGB, 1, depthFramebufferFlag));
					}
					CATCH_ALL_AND_PRINT_WARNING_AND(throw;)
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
					*cfg::opengl::vidOutline &&
					*cfg::opengl::vidShader &&
					haveArbFragmentShader &&
					haveArbShaderObjects &&
					haveArbVertexShader &&
					haveExtFramebufferObject;
			}
		}
	}
}
