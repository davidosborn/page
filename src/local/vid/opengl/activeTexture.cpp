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
			GLenum GetActiveTexture()
			{
				assert(haveArbMultitexture);
				return GetInteger(GL_ACTIVE_TEXTURE_ARB);
			}
			unsigned GetActiveTextureIndex()
			{
				return GetActiveTexture() - GL_TEXTURE0_ARB;
			}
			bool CanAllocActiveTexture()
			{
				assert(haveArbMultitexture);
				return GetActiveTextureIndex() + !reset < GetInteger(GL_MAX_TEXTURE_UNITS_ARB);
			}

			// modifiers
			void AllocActiveTexture()
			{
				assert(haveArbMultitexture);
				assert(CanAllocActiveTexture());
				glActiveTextureARB(GetActiveTexture() + !reset);
				reset = false;
			}
			void ResetActiveTexture(GLenum activeTexture)
			{
				assert(haveArbMultitexture);
				glActiveTextureARB(activeTexture);
				reset = true;
			}
		}
	}
}
