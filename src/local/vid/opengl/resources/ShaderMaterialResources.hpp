/**
 * @section license
 *
 * Copyright (c) 2006-2013 David Osborn
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
