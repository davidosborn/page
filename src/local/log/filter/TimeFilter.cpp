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
#include "../../util/sstream.hpp" // make_ostringstream
#include "TimeFilter.hpp"

namespace page
{
	namespace log
	{
		TimeFilter::TimeFilter() {}
		TimeFilter::TimeFilter(const std::shared_ptr<Stream> &link) :
			PrefixFilter(link) {}

		void TimeFilter::PutPrefix()
		{
			static std::time_t time;
			std::time_t newTime = std::time(0);
			if (newTime != time)
			{
				time = newTime;
				MAKE_OSTRINGSTREAM_1(char)//auto ss(util::make_ostringstream<char>());
				const char timePattern[] = "%X:\n";
				std::use_facet<std::time_put<char>>(ss.getloc()).put(
					ss, ss, ss.fill(), std::localtime(&time),
					timePattern, timePattern + sizeof timePattern - 1);
				GateFilter::Put(ss.str().data(), ss.str().size());
				sync();
			}
			std::string indent(2, ' ');
			GateFilter::Put(indent.data(), indent.size());
		}
	}
}
