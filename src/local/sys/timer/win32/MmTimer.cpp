#include "../../../err/Exception.hpp"
#include "MmTimer.hpp"

namespace page
{
	namespace sys
	{
		namespace win32
		{
			// construct/destroy
			MmTimer::MmTimer()
			{
				if (timeBeginPeriod(1))
					THROW((err::Exception<err::SysModuleTag, err::WinPlatform32>("failed to set multimedia timer resolution")))
				time = timeGetTime();
			}
			MmTimer::~MmTimer()
			{
				timeEndPeriod(1);
			}

			// update
			float MmTimer::Step()
			{
				DWORD newTime = timeGetTime();
				float delta = (time - newTime) / 1000;
				time = newTime;
				return delta;
			}
		}

		// factory function
		Timer *MakeTimer()
		{
			return new win32::MmTimer;
		}
	}
}
