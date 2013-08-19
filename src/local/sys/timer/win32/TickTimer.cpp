#include "TickTimer.hpp"

namespace page
{
	namespace sys
	{
		namespace win32
		{
			// construct
			TickTimer::TickTimer() : time(GetTickCount()) {}

			// update
			float TickTimer::Step()
			{
				DWORD newTime = GetTickCount();
				float delta = (newTime - time) / 1000;
				time = newTime;
				return delta;
			}
		}

		// factory function
		Timer *MakeTimer()
		{
			return new win32::TickTimer;
		}
	}
}
