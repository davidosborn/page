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
