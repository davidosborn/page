#ifndef    page_local_vid_opengl_FramebufferFlags_hpp
#   define page_local_vid_opengl_FramebufferFlags_hpp

namespace page
{
	namespace vid
	{
		namespace opengl
		{
			enum FramebufferFlags
			{
				noneFramebufferFlag   = 0x0,
				depthFramebufferFlag  = 0x1,
				filterFramebufferFlag = 0x2,
				mipmapFramebufferFlag = 0x4
			};
		}
	}
}

#endif
