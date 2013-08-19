// render target binding state saver
// also saves scissor and viewport state
// NOP for framebuffer if EXT_framebuffer_object not supported

#ifndef    page_local_vid_opengl_RenderTargetSaver_hpp
#   define page_local_vid_opengl_RenderTargetSaver_hpp

#	include <GL/gl.h> // GL{enum,{,u}int}

#	include "../../util/StateSaver.hpp"

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			struct RenderTargetSaver : util::StateSaver<RenderTargetSaver>
			{
				friend class util::StateSaver<RenderTargetSaver>;
				typedef util::StateSaver<RenderTargetSaver> Base;

				// construct
				explicit RenderTargetSaver(bool enabled = true);

				private:
				// modifiers
				void Save();
				void Load();

				GLuint framebuffer;
				GLenum drawBuffer;
				GLint scissorBox[4];
				GLint viewport[4];
			};
		}
	}
}

#endif
