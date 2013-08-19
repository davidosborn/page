#ifndef    page_local_sys_timer_win32_PcTimer_hpp
#   define page_local_sys_timer_win32_PcTimer_hpp

#	include <windows.h> // LARGE_INTEGER

#	include "../../Timer.hpp"

namespace page
{
	namespace sys
	{
		namespace win32
		{
			struct PcTimer : Timer
			{
				// construct
				PcTimer();

				private:
				// update
				float Step();

				LARGE_INTEGER freq, time;
			};
		}
	}
}

#endif
