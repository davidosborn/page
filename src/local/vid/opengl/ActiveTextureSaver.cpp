#include "activeTexture.hpp" // {Get,Reset}ActiveTexture
#include "ActiveTextureSaver.hpp"
#include "ext.hpp" // ARB_multitexture

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			// construct
			ActiveTextureSaver::ActiveTextureSaver(bool enabled)
			{
				Reset(enabled);
			}

			// modifiers
			void ActiveTextureSaver::Save()
			{
				if (haveArbMultitexture) texture = GetActiveTexture();
			}
			void ActiveTextureSaver::Load()
			{
				if (haveArbMultitexture) ResetActiveTexture(texture);
			}
		}
	}
}
