#ifndef    page_local_sys_timer_win32_TickTimer_hpp
#   define page_local_sys_timer_win32_TickTimer_hpp

#	include <windows.h> // DWORD

#	include "../../Timer.hpp"

namespace page
{
	namespace sys
	{
		namespace win32
		{
			struct TickTimer : Timer
			{
				// construct
				TickTimer();

				private:
				// update
				float Step();

				DWORD time;
			};
		}
	}
}

#endif
