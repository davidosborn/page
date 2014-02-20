/**
 * @copyright
 *
 * Copyright (c) 2006-2014 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the same place
 *    and form as other copyright, license, and disclaimer information.
 *
 * 3. Redistributions in binary form must also include an acknowledgement in the
 *    same place and form as other acknowledgements (such as the credits),
 *    similar in substance to the following:
 *
 *       Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#include <cassert>

#include <GL/gl.h>

#include "../ext.hpp" // ARB_shader_objects
#include "../Program.hpp" // Bind, Program::GetUniform
#include "../ProgramSaver.hpp"
#include "../Resources.hpp" // Resources::{color2grayProgram,{Get,Has}Program}
#include "SaturationFilter.hpp"

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			// construct/destroy
			SaturationFilter::SaturationFilter(DrawContext &context, Resources &res, float saturation, bool opaque) :
				Filter(context, res, opaque), saturation(saturation)
			{
				assert(Check(res));
			}
			SaturationFilter::~SaturationFilter()
			{
				PrepComposite();
				RestoreRenderTargetBinding();
				ProgramSaver programSaver;
				const Program &color2grayProgram(
					GetResources().GetProgram(Resources::color2grayProgram));
				Bind(color2grayProgram);
				glUniform1fARB(color2grayProgram.GetUniform("grayness").location, 1 - saturation);
				Fill(GetRenderTarget());
			}

			// compatibility
			bool SaturationFilter::Check(Resources &res)
			{
				return
					Filter::Check(res) &&
					res.HasProgram(Resources::color2grayProgram);
			}
		}
	}
}
