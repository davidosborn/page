#include <ctime> // {,local}time
#include <locale> // use_facet, time_put
#include <sstream> // ostringstream

#include "../../cfg/vars.hpp"
#include "TimeFilter.hpp"

namespace page
{
	namespace log
	{
		/*----------------------------+
		| PrefixFilter implementation |
		+----------------------------*/

		std::string TimeFilter::GetPrefix() const
		{
			static std::time_t lastTime = 0;
			static std::string lastTimeString;

			// update time string if time has changed
			bool timeChanged = false;
			std::time_t newTime = std::time(0);
			if (newTime != lastTime)
			{
				std::ostringstream ss;
				const char timePattern[] = "%X";
				std::use_facet<std::time_put<char>>(ss.getloc()).put(
					ss, ss, ss.fill(), std::localtime(&newTime),
					timePattern, timePattern + sizeof timePattern - 1);

				lastTime = newTime;
				lastTimeString = ss.str();
				timeChanged = true;
			}

			// generate prefix using time string
			if (*CVAR(logTimeChange))
			{
				std::string r;
				if (timeChanged)
					r += lastTimeString + ":\n";
				r += std::string(2, ' ');
				return r;
			}
			return lastTimeString + ": ";
		}
	}
}
