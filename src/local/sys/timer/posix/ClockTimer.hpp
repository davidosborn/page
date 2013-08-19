#ifndef    page_local_sys_timer_posix_ClockTimer_hpp
#   define page_local_sys_timer_posix_ClockTimer_hpp

#	include <time.h> // timespec

#	include "../../Timer.hpp"

namespace page
{
	namespace sys
	{
		namespace posix
		{
			struct ClockTimer : Timer
			{
				private:
				// update
				float Step();

				timespec time;
			};
		}
	}
}

#endif
