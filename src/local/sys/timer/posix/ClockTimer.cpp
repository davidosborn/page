#include "../../../err/Exception.hpp"
#include "ClockTimer.hpp"

namespace page
{
	namespace sys
	{
		namespace posix
		{
			// update
			float ClockTimer::Step()
			{
				timespec newTime;
				if (clock_gettime(CLOCK_REALTIME, &newTime) == -1)
					THROW((err::Exception<err::SysModuleTag, err::PosixPlatformTag>("failed to get real-time clock time")))
				float delta = newTime.tv_sec - time.tv_sec +
					(newTime.tv_nsec - time.tv_nsec) / 1000000000.f;
				time = newTime;
				return delta;
			}
		}

		// factory function
		Timer *MakeTimer()
		{
			return new posix::ClockTimer;
		}
	}
}
