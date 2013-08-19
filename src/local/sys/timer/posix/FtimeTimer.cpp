#include "../../../err/Exception.hpp"
#include "FtimeTimer.hpp"

namespace page
{
	namespace sys
	{
		namespace posix
		{
			// update
			float FtimeTimer::Step()
			{
				timeb newTime;
				if (ftime(&newTime) == -1)
					THROW((err::Exception<err::SysModuleTag, err::PosixPlatformTag>("failed to get system time")))
				float delta = newTime.time - time.time +
					(newTime.millitm - time.millitm) / 1000000.f;
				time = newTime;
				return delta;
			}
		}

		// factory function
		Timer *MakeTimer()
		{
			return new posix::FtimeTimer;
		}
	}
}
