#include <cassert>

#include <GL/gl.h>

#include "ext.hpp" // ARB_multitexture
#include "get.hpp" // GetInteger

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			namespace
			{
				bool reset = true;
			}

			// state
			GLenum GetClientActiveTexture()
			{
				assert(haveArbMultitexture);
				return GetInteger(GL_CLIENT_ACTIVE_TEXTURE_ARB);
			}
			unsigned GetClientActiveTextureIndex()
			{
				return GetClientActiveTexture() - GL_TEXTURE0_ARB;
			}
			bool CanAllocClientActiveTexture()
			{
				assert(haveArbMultitexture);
				return GetClientActiveTextureIndex() + !reset < GetInteger(GL_MAX_TEXTURE_UNITS_ARB);
			}

			// modifiers
			void AllocClientActiveTexture()
			{
				assert(haveArbMultitexture);
				assert(CanAllocClientActiveTexture());
				glClientActiveTextureARB(GetClientActiveTexture() + !reset);
				reset = false;
			}
			void ResetClientActiveTexture(GLenum clientActiveTexture)
			{
				assert(haveArbMultitexture);
				glClientActiveTextureARB(clientActiveTexture);
				reset = true;
			}
		}
	}
}
