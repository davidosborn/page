#ifndef    page_local_sys_timer_ClockTimer_hpp
#   define page_local_sys_timer_ClockTimer_hpp

#	include <ctime> // clock_t

#	include "../Timer.hpp"

namespace page
{
	namespace sys
	{
		struct ClockTimer : Timer
		{
			// construct
			ClockTimer();

			private:
			// update
			float Step();

			std::clock_t time;
		};
	}
}

#endif
