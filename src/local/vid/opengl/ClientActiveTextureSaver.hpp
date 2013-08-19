// client active texture state saver
// NOP if ARB_multitexture not supported

#ifndef    page_local_vid_opengl_ClientActiveTextureSaver_hpp
#   define page_local_vid_opengl_ClientActiveTextureSaver_hpp

#	include <GL/gl.h> // GLenum

#	include "../../util/StateSaver.hpp"

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			struct ClientActiveTextureSaver : util::StateSaver<ClientActiveTextureSaver>
			{
				friend class util::StateSaver<ClientActiveTextureSaver>;
				typedef util::StateSaver<ClientActiveTextureSaver> Base;

				// construct
				explicit ClientActiveTextureSaver(bool enabled = true);

				private:
				// modifiers
				void Save();
				void Load();

				GLenum texture;
			};
		}
	}
}

#endif
