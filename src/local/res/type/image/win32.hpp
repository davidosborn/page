#ifndef    page_local_res_type_image_win32_hpp
#   define page_local_res_type_image_win32_hpp

#	include <windows.h> // HBITMAP

namespace page
{
	namespace res
	{
		class Image;

		namespace win32
		{
			HBITMAP MakeBitmap(const Image &);
		}
	}
}

#endif
