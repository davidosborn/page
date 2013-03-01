/**
 * @section license
 *
 * Copyright (c) 2006-2012 David Osborn
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

#include <ctime> // localtime, time
#include <iostream> // cerr, clog, cout
#include <locale> // locale, use_facet, time_put

#include "../cfg/vars.hpp"
#include "../util/ios.hpp" // InitIos
#include "../util/locale.hpp" // InitLocale
#include "../util/cpp.hpp" // STRINGIZE
#include "Indenter.hpp"

namespace page
{
	namespace log
	{
		void PrintInit()
		{
			// initialize default locale and stream state
			/*util::InitLocale();
			std::cout.imbue(std::locale()); util::InitIos(std::cout);
			std::cerr.imbue(std::locale()); util::InitIos(std::cerr);
			std::clog.imbue(std::locale()); util::InitIos(std::clog);*/

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
