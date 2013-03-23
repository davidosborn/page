/**
 * @section license
 *
 * Copyright (c) 2006-2013 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution, and in the same
 *    place and form as other copyright, license, and disclaimer information.
 *
 * As a special exception, distributions of derivative works in binary form may
 * include an acknowledgement in place of the above copyright notice, this list
 * of conditions, and the following disclaimer in the documentation and/or other
 * materials provided with the distribution, and in the same place and form as
 * other acknowledgements, similar in substance to the following:
 *
 *    Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

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
