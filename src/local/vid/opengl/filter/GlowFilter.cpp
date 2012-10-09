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

#include <cassert>
#include <GL/gl.h>
#include "../../../math/interp.hpp" // HermiteConvolutionKernel
#include "../blend.hpp" // ColorBlendFunc
#include "../ext.hpp" // ARB_shader_objects
#include "../Framebuffer.hpp" // Framebuffer::GetPow2Size
#include "../Program.hpp" // Bind, Program::GetUniform
#include "../ProgramSaver.hpp"
#include "../RenderTarget.hpp" // Bind, RenderTarget::framebuffer
#include "../Resources.hpp" // Resources::{color2{alpha,gray}Program,convolutionFilter5[hv]Program,{Get,Has}{Program,RenderTargetPool},rgb{,a}BlurRenderTargetPool}
#include "GlowFilter.hpp"

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			// construct/destroy
			GlowFilter::GlowFilter(DrawContext &context, Resources &res, float intensity, bool opaque) :
				Filter(context, res, opaque), intensity(intensity)
			{
				assert(Check(res));
			}
			GlowFilter::~GlowFilter()
			{
				const Resources &res(GetResources());
				const Program
					&luminanceProgram(res.GetProgram(IsOpaque() ?
						Resources::color2grayProgram :
						Resources::color2alphaProgram)),
					&blurhProgram(res.GetProgram(Resources::convolutionFilter5hProgram)),
					&blurvProgram(res.GetProgram(Resources::convolutionFilter5vProgram));
				const RenderTargetPool &blurRenderTargetPool(
					res.GetRenderTargetPool(IsOpaque() ?
						Resources::rgbBlurRenderTargetPool :
						Resources::rgbaBlurRenderTargetPool));
				RenderTargetPool::Pooled blurRenderTargets[] =
				{
					blurRenderTargetPool.Get(),
					blurRenderTargetPool.Get()
				};
				// copy to destination
				PrepComposite();
				RestoreRenderTargetBinding();
				Fill(GetRenderTarget());
				// perform desaturation
				Bind(GetContext(), blurRenderTargets[0]);
				ProgramSaver programSaver;
				Bind(luminanceProgram);
				glDisable(GL_BLEND);
				Fill(GetRenderTarget());
				// perform horizontal blur
				Bind(GetContext(), blurRenderTargets[1]);
				Bind(blurhProgram);
				glUniform1fvARB(
					blurhProgram.GetUniform("kernel").location, 5,
					&*math::HermiteConvolutionKernel<5>(3.f).begin());
				glUniform1iARB(
					blurhProgram.GetUniform("texSize").location,
					blurRenderTargetPool.GetPow2Size().x);
				Fill(*blurRenderTargets[0]);
				// perform vertical blur and add to destination
				RestoreRenderTargetBinding();
				Bind(blurvProgram);
				glUniform1fvARB(
					blurvProgram.GetUniform("kernel").location, 5,
					&*math::HermiteConvolutionKernel<5>(3.f).begin());
				glUniform1iARB(
					blurvProgram.GetUniform("texSize").location,
					blurRenderTargetPool.GetPow2Size().y);
				ColorBlendFunc(GL_SRC_ALPHA, GL_ONE);
				glEnable(GL_BLEND);
				glPushAttrib(GL_CURRENT_BIT);
				glColor4f(1, 1, 1, intensity);
				Fill(*blurRenderTargets[1]);
			}

			// compatibility
			bool GlowFilter::Check(Resources &res)
			{
				return
					Filter::Check(res) &&
					res.HasProgram(Resources::color2alphaProgram) &&
					res.HasProgram(Resources::color2grayProgram) &&
					res.HasProgram(Resources::convolutionFilter5hProgram) &&
					res.HasProgram(Resources::convolutionFilter5vProgram) &&
					res.HasRenderTargetPool(Resources::rgbBlurRenderTargetPool) &&
					res.HasRenderTargetPool(Resources::rgbaBlurRenderTargetPool);
			}
		}
	}
}
