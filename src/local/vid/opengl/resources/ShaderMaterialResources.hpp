#ifndef    page_local_vid_opengl_resources_ShaderMaterialResources_hpp
#   define page_local_vid_opengl_resources_ShaderMaterialResources_hpp

#	include "../../../cache/proxy/opengl/Program.hpp"
#	include "../../../res/type/Material.hpp" // Material::Pass

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			class Resources;

			struct ShaderMaterialResources
			{
				// construct
				explicit ShaderMaterialResources(const Resources &res);

				// program generation
				enum MaterialMask
				{
					noneMaterialMask     = 0x000,
					ambientMaterialMask  = 0x001,
					diffuseMaterialMask  = 0x002,
					emissiveMaterialMask = 0x004,
					fresnelMaterialMask  = 0x008,
					glossMaterialMask    = 0x010,
					maskMaterialMask     = 0x020,
					normalMaterialMask   = 0x040,
					specularMaterialMask = 0x080,
					shadowMaterialMask   = 0x100,
					allMaterialMask      = 0xfff
				};
				cache::opengl::Program GetProgram(
					const res::Material::Pass &,
					MaterialMask = allMaterialMask,
					bool fallback = false) const;

				// compatibility
				static bool Check();

				private:
				const Resources &res;
			};
		}
	}
}

#endif
