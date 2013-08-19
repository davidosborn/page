#ifndef    page_local_sys_timer_posix_TimedayTimer_hpp
#   define page_local_sys_timer_posix_TimedayTimer_hpp

#	include <sys/time.h> // timeval

#	include "../../Timer.hpp"

namespace page
{
	namespace sys
	{
		namespace posix
		{
			struct TimedayTimer : Timer
			{
				private:
				// update
				float Step();

				timeval time;
			};
		}
	}
}

#endif
