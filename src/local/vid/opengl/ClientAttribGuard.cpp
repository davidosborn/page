#include <cassert>

#include <GL/gl.h>

#include "ClientAttribGuard.hpp"
#include "get.hpp" // GetInteger

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			// construct
			ClientAttribGuard::ClientAttribGuard(bool enabled)
			{
				Reset(enabled);
			}

			// modifiers
			void ClientAttribGuard::Save()
			{
				depth = GetInteger(GL_CLIENT_ATTRIB_STACK_DEPTH);
			}
			void ClientAttribGuard::Load()
			{
				int n = GetInteger(GL_CLIENT_ATTRIB_STACK_DEPTH) - depth;
				assert(n >= 0);
				while (n--) glPopClientAttrib();
			}
		}
	}
}
