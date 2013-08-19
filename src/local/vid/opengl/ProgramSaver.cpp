#include "ext.hpp" // ARB_shader_objects
#include "ProgramSaver.hpp"

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			// construct/destroy
			ProgramSaver::ProgramSaver(bool enabled)
			{
				Reset(enabled);
			}

			// modifiers
			void ProgramSaver::Save()
			{
				if (haveArbShaderObjects)
					program = glGetHandleARB(GL_PROGRAM_OBJECT_ARB);
			}
			void ProgramSaver::Load()
			{
				if (haveArbShaderObjects) glUseProgramObjectARB(program);
			}
		}
	}
}
