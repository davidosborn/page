#include "clientActiveTexture.hpp" // {Get,Reset}ClientActiveTexture
#include "ClientActiveTextureSaver.hpp"
#include "ext.hpp" // ARB_multitexture

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			// construct/destroy
			ClientActiveTextureSaver::ClientActiveTextureSaver(bool enabled)
			{
				Reset(enabled);
			}

			// modifiers
			void ClientActiveTextureSaver::Save()
			{
				if (haveArbMultitexture) texture = GetClientActiveTexture();
			}
			void ClientActiveTextureSaver::Load()
			{
				if (haveArbMultitexture) ResetClientActiveTexture(texture);
			}
		}
	}
}
