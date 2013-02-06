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

#ifndef    page_local_vid_opengl_Resources_hpp
#   define page_local_vid_opengl_Resources_hpp

#	include <array>
#	include <memory> // {unique,shared}_ptr
#	include "../../math/fwd.hpp" // Vector
#	include "../../util/NonCopyable.hpp"

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			class Program;
			class RenderTargetPool;
			class ShaderMaterialResources;
			class ShaderOutlineResources;
			class ShadowResources;
			class Texture;

			struct Resources : util::NonCopyable
			{
				// construct
				explicit Resources(const math::Vector<2, unsigned> &size);

				// modifiers
				void Resize(const math::Vector<2, unsigned> &size);

				// general-purpose programs
				enum Program
				{
					color2alphaProgram,
					color2grayProgram,
					convolutionFilter3hProgram,
					convolutionFilter3vProgram,
					convolutionFilter5hProgram,
					convolutionFilter5vProgram,
					expConvolutionFilter3hProgram,
					expConvolutionFilter3vProgram,
					expConvolutionFilter5hProgram,
					expConvolutionFilter5vProgram,
					expMeanFilter3hProgram,
					expMeanFilter3vProgram,
					expMeanFilter5hProgram,
					expMeanFilter5vProgram,
					meanFilter3hProgram,
					meanFilter3vProgram,
					meanFilter5hProgram,
					meanFilter5vProgram,
					medianFilter3Program,
					normalProgram,
					_programSentinel // end marker
				};
				bool HasProgram(Program) const;
				const opengl::Program &GetProgram(Program) const;

				// general-purpose render-target pools
				enum RenderTargetPool
				{
					rgbBlurRenderTargetPool,
					rgbaBlurRenderTargetPool,
					rgbCompositeRenderTargetPool,
					rgbaCompositeRenderTargetPool,
					_renderTargetPoolSentinel // end marker
				};
				bool HasRenderTargetPool(RenderTargetPool) const;
				const opengl::RenderTargetPool &GetRenderTargetPool(RenderTargetPool) const;

				// general-purpose textures
				enum Texture
				{
					blackTexture,
					blueTexture,
					transparentTexture,
					whiteTexture,
					_textureSentinel // end marker
				};
				bool HasTexture(Texture) const;
				const opengl::Texture &GetTexture(Texture) const;

				// shader-outline resources
				typedef ShaderOutlineResources ShaderOutline;
				bool HasShaderOutline() const;
				const ShaderOutline &GetShaderOutline() const;

				// shader-render resources
				typedef ShaderMaterialResources ShaderMaterial;
				bool HasShaderMaterial() const;
				const ShaderMaterial &GetShaderMaterial() const;

				// shadow resources
				typedef ShadowResources Shadow;
				bool HasShadow() const;
				const Shadow &GetShadow() const;

				private:
				static const unsigned
					numPrograms          = _programSentinel,
					numRenderTargetPools = _renderTargetPoolSentinel,
					numTextures          = _textureSentinel;

				// general-purpose resources
				typedef std::array<std::shared_ptr<opengl::Program>,          numPrograms>          Programs;
				typedef std::array<std::shared_ptr<opengl::RenderTargetPool>, numRenderTargetPools> RenderTargetPools;
				typedef std::array<std::shared_ptr<opengl::Texture>,          numTextures>          Textures;
				Programs          programs;
				RenderTargetPools renderTargetPools;
				Textures          textures;
				// specialized resources
				std::unique_ptr<ShaderOutline> shaderOutline;
				std::unique_ptr<ShaderMaterial> shaderMaterial;
				std::unique_ptr<Shadow> shadow;
			};
		}
	}
}

#endif
