#ifndef    page_local_vid_opengl_activeTexture_hpp
#   define page_local_vid_opengl_activeTexture_hpp

#	include <GL/gl.h> // GLenum, glext.h
#	include <GL/glext.h> // GL_TEXTURE0_ARB

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			// state
			GLenum GetActiveTexture();
			unsigned GetActiveTextureIndex();
			bool CanAllocActiveTexture();

			// modifiers
			void AllocActiveTexture();
			void ResetActiveTexture(GLenum = GL_TEXTURE0_ARB);
		}
	}
}

#endif
