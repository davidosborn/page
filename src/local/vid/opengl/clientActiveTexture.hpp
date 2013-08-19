#ifndef    page_local_vid_opengl_clientActiveTexture_hpp
#   define page_local_vid_opengl_clientActiveTexture_hpp

#	include <GL/gl.h> // GLenum, glext.h
#	include <GL/glext.h> // GL_TEXTURE0_ARB

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			// state
			GLenum GetClientActiveTexture();
			unsigned GetClientActiveTextureIndex();
			bool CanAllocClientActiveTexture();

			// modifiers
			void AllocClientActiveTexture();
			void ResetClientActiveTexture(GLenum = GL_TEXTURE0_ARB);
		}
	}
}

#endif
