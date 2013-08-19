#include "../err/Exception.hpp"
#include "ClockTimer.hpp"

namespace page
{
	namespace sys
	{
		// construct
		ClockTimer::ClockTimer()
		{
			if ((time = std::clock()) == -1)
				THROW((err::Exception<err::SysModuleTag, err::NotAvailableTag>("clock device not available")))
		}

		// update
		float ClockTimer::Step()
		{
			std::clock_t newTime = std::clock();
			if (newTime == -1)
				THROW((err::Exception<err::SysModuleTag, err::NotAvailableTag>("clock device not available")))
			float delta = (newTime - time) / CLOCKS_PER_SEC;
			time = newTime;
			return delta;
		}

		// factory function
		Timer *MakeTimer()
		{
			return new ClockTimer;
		}
	}
}
