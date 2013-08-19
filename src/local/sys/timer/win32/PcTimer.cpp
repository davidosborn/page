#include "../../../err/Exception.hpp"
#include "PcTimer.hpp"

namespace page
{
	namespace sys
	{
		namespace win32
		{
			// construct
			PcTimer::PcTimer()
			{
				if (!QueryPerformanceFrequency(&freq))
					THROW((err::Exception<err::SysModuleTag, err::Win32PlatformTag>("failed to initialize performance counter")))
				if (!QueryPerformanceCounter(&time))
					THROW((err::Exception<err::SysModuleTag, err::Win32PlatformTag>("failed to query performance counter")))
			}

			// update
			float PcTimer::Step()
			{
				LARGE_INTEGER newTime;
				if (!QueryPerformanceCounter(&newTime))
					THROW((err::Exception<err::SysModuleTag, err::Win32PlatformTag>("failed to query performance counter")))
				float delta = static_cast<long double>(newTime.QuadPart - time.QuadPart) / freq.QuadPart;
				time = newTime;
				return delta;
			}
		}

		// factory function
		Timer *MakeTimer()
		{
			return new win32::PcTimer;
		}
	}
}
