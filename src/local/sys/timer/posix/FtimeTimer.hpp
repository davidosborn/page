#ifndef    page_local_sys_timer_posix_FtimeTimer_hpp
#   define page_local_sys_timer_posix_FtimeTimer_hpp

#	include <sys/timeb.h> // timeb

#	include "../../Timer.hpp"

namespace page
{
	namespace sys
	{
		namespace posix
		{
			struct FtimeTimer : Timer
			{
				private:
				// update
				float Step();

				timeb time;
			};
		}
	}
}

#endif
