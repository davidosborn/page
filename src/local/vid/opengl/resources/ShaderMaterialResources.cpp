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

#include <cassert>

#include "../../../cache/proxy/Resource.hpp"
#include "../../../cfg/vars.hpp"
#include "../ext.hpp" // ARB_{{fragment,vertex}_shader,shader_objects}
#include "../Resources.hpp" // Resources::{Get,Has}Shadow
#include "ShaderMaterialResources.hpp"
#include "ShadowResources.hpp" // ShadowResources::GetType

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			// construct
			ShaderMaterialResources::ShaderMaterialResources(const Resources &res) :
				res(res) {}

			// render program generation
			cache::opengl::Program ShaderMaterialResources::GetProgram(const res::Material::Pass &pass, MaterialMask mask, bool fallback) const
			{
				assert(Check());
				// FIXME: implement fallback
				if (mask & shadowMaterialMask && res.HasShadow())
				{
					switch (res.GetShadow().GetType())
					{
						case Resources::Shadow::exponentialType:
						return cache::opengl::Program({
							cache::Resource<res::opengl::Shader>("shader/glsl/render/render.fs"),
							cache::Resource<res::opengl::Shader>("shader/glsl/render/render.vs"),
							cache::Resource<res::opengl::Shader>("shader/glsl/render/render-shadow.fs"),
							cache::Resource<res::opengl::Shader>("shader/glsl/render/render-shadow.vs"),
							cache::Resource<res::opengl::Shader>("shader/glsl/render/render-shadow-exponential.fs"),
							cache::Resource<res::opengl::Shader>("shader/glsl/render/sample-diffuse.fs"),
							cache::Resource<res::opengl::Shader>("shader/glsl/render/sample-mask.fs")});
						break;
						case Resources::Shadow::packedType:
						// FIXME: implement
						break;
						case Resources::Shadow::varianceType:
						return cache::opengl::Program({
							cache::Resource<res::opengl::Shader>("shader/glsl/render/render.fs"),
							cache::Resource<res::opengl::Shader>("shader/glsl/render/render.vs"),
							cache::Resource<res::opengl::Shader>("shader/glsl/render/render-shadow.fs"),
							cache::Resource<res::opengl::Shader>("shader/glsl/render/render-shadow.vs"),
							cache::Resource<res::opengl::Shader>("shader/glsl/render/render-shadow-variance.fs"),
							cache::Resource<res::opengl::Shader>("shader/glsl/render/sample-diffuse.fs"),
							cache::Resource<res::opengl::Shader>("shader/glsl/render/sample-mask.fs")});
						break;
						default: assert(!"invalid shadow type");
					}
				}
				else
				{
					return cache::opengl::Program({
						cache::Resource<res::opengl::Shader>("shader/glsl/render/render.fs"),
						cache::Resource<res::opengl::Shader>("shader/glsl/render/render.vs"),
						cache::Resource<res::opengl::Shader>("shader/glsl/render/render-shadow-none.fs"),
						cache::Resource<res::opengl::Shader>("shader/glsl/render/render-shadow-none.vs"),
						cache::Resource<res::opengl::Shader>("shader/glsl/render/sample-diffuse.fs"),
						cache::Resource<res::opengl::Shader>("shader/glsl/render/sample-mask.fs")});
				}
			}

			// compatibility
			bool ShaderMaterialResources::Check()
			{
				return
					*CVAR(opengl)::renderShader &&
					haveArbFragmentShader &&
					haveArbShaderObjects &&
					haveArbVertexShader;
			}
		}
	}
}
