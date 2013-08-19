#include "TimedayTimer.hpp"

namespace page
{
	namespace sys
	{
		namespace posix
		{
			// update
			float TimedayTimer::Step()
			{
				timeval newTime;
				gettimeofday(&newTime, 0);
				float delta = newTime.tv_sec - time.tv_sec +
					(newTime.tv_usec - time.tv_usec) / 1000000.f;
				time = newTime;
				return delta;
			}
		}

		// factory function
		Timer *MakeTimer()
		{
			return new posix::TimedayTimer;
		}
	}
}
