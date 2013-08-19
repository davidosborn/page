#ifndef    page_local_sys_timer_win32_MmTimer_hpp
#   define page_local_sys_timer_win32_MmTimer_hpp

#	include <windows.h> // DWORD

#	include "../../Timer.hpp"

namespace page
{
	namespace sys
	{
		namespace win32
		{
			struct MmTimer : Timer
			{
				// construct/destroy
				MmTimer();
				~MmTimer();

				private:
				// update
				float Step();

				DWORD time;
			};
		}
	}
}

#endif
