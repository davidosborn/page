#include <cassert>

#include <GL/gl.h>

#include "AttribGuard.hpp"
#include "get.hpp" // GetInteger

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			// construct
			AttribGuard::AttribGuard(bool enabled)
			{
				Reset(enabled);
			}

			// modifiers
			void AttribGuard::Save()
			{
				depth = GetInteger(GL_ATTRIB_STACK_DEPTH);
			}
			void AttribGuard::Load()
			{
				int n = GetInteger(GL_ATTRIB_STACK_DEPTH) - depth;
				assert(n >= 0);
				while (n--) glPopAttrib();
			}
		}
	}
}
