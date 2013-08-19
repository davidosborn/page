#include <ctime> // localtime, time
#include <iostream> // cerr, clog, cout
#include <locale> // use_facet, time_put

#include "../cfg/vars.hpp"
#include "../util/cpp.hpp" // STRINGIZE
#include "Indenter.hpp"

namespace page
{
	namespace log
	{
		void PrintInit()
		{
			// print current date/time
			std::time_t time = std::time(0);
			std::cout << "execution initiated on ";
			const char timePattern[] = "%x at %X";
			std::use_facet<std::time_put<char>>(std::cout.getloc()).put(
				std::cout, std::cout, std::cout.fill(), std::localtime(&time),
				timePattern, timePattern + sizeof timePattern - 1);
			std::cout << std::endl;

			// print program information
			std::cout << "this is " STRINGIZE(NAME) " " STRINGIZE(VERSION) ": " STRINGIZE(LONGNAME) << std::endl;
			log::Indenter indenter;
			std::cout << "built on " STRINGIZE(BUILD_DATE) " at " STRINGIZE(BUILD_TIME) << std::endl;
#if HOST != BUILD
			std::cout << "cross-compiled under " STRINGIZE(BUILD) " for " STRINGIZE(HOST) << std::endl;
#endif
		}

		void PrintStats()
		{
			// print statistics
			if (*CVAR(logStats))
			{
				// FIXME: implement
			}
		}
	}
}
