#ifndef    page_local_res_type_cursor_win32_hpp
#   define page_local_res_type_cursor_win32_hpp

#	include <windows.h> // HCURSOR

namespace page
{
	namespace res
	{
		class Cursor;

		namespace win32
		{
			HCURSOR MakeWin32Cursor(const Cursor &, unsigned size);
		}
	}
}

#endif
