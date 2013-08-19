// active texture state saver
// NOP if ARB_multitexture not supported

#ifndef    page_local_vid_opengl_ActiveTextureSaver_hpp
#   define page_local_vid_opengl_ActiveTextureSaver_hpp

#	include <GL/gl.h> // GLenum

#	include "../../util/StateSaver.hpp"

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			struct ActiveTextureSaver : util::StateSaver<ActiveTextureSaver>
			{
				friend class util::StateSaver<ActiveTextureSaver>;
				typedef util::StateSaver<ActiveTextureSaver> Base;

				// construct
				explicit ActiveTextureSaver(bool enabled = true);

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
