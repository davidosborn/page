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
