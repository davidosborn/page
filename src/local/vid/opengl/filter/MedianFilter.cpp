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
#include "../ext.hpp" // ARB_shader_objects
#include "../Framebuffer.hpp" // Framebuffer::GetPow2Size
#include "../Program.hpp" // Bind, Program::GetUniform
#include "../ProgramSaver.hpp"
#include "../RenderTarget.hpp" // RenderTarget::framebuffer
#include "../Resources.hpp" // Resources::{{Get,Has}{Program,RenderTargetPool},medianFilter3Program}
#include "MedianFilter.hpp"

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			// construct/destroy
			MedianFilter::MedianFilter(DrawContext &context, Resources &res, unsigned passes, bool opaque) :
				Filter(context, res, opaque), passes(passes)
			{
				assert(Check(res));
			}
			MedianFilter::~MedianFilter()
			{
				// FIXME: implement multipass
				PrepComposite();
				RestoreRenderTargetBinding();
				ProgramSaver programSaver;
				const Program &medianFilter3Program(
					GetResources().GetProgram(Resources::medianFilter3Program));
				Bind(medianFilter3Program);
				glUniform2ivARB(medianFilter3Program.GetUniform("texSize").location, 1,
					&*math::Vector<2, GLint>(GetRenderTarget().framebuffer.GetPow2Size()).begin());
				Fill(GetRenderTarget());
			}

			// compatibility
			bool MedianFilter::Check(Resources &res)
			{
				return
					Filter::Check(res) &&
					res.HasProgram(Resources::medianFilter3Program);
			}
		}
	}
}
