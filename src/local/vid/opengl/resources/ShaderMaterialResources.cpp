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
