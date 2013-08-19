#ifndef    page_local_vid_opengl_TextureFlags_hpp
#   define page_local_vid_opengl_TextureFlags_hpp

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			enum TextureFlags
			{
				noneTextureFlag   = 0x0,
				filterTextureFlag = 0x1,
				mipmapTextureFlag = 0x2
			};
		}
	}
}

#endif
