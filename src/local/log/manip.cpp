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

#include <iostream>
#include <typeinfo> // bad_cast
#include "filter/IndentFilter.hpp"
#include "Stream.hpp"

namespace page
{
	namespace log
	{
		std::ostream &Indent(std::ostream &os)
		{
			IndentFilter::Indent();
			return os;
		}
		std::ostream &Dedent(std::ostream &os)
		{
			IndentFilter::Dedent();
			return os;
		}

		std::ostream &Clear(std::ostream &os)
		{
			try
			{
				dynamic_cast<Stream &>(*std::cout.rdbuf()).Clear();
				dynamic_cast<Stream &>(*std::cerr.rdbuf()).Clear();
			}
			catch (const std::bad_cast &) {}
			return os;
		}
		std::ostream &Reset(std::ostream &os)
		{
			try
			{
				dynamic_cast<Stream &>(*std::cout.rdbuf()).Reset();
				dynamic_cast<Stream &>(*std::cerr.rdbuf()).Reset();
			}
			catch (const std::bad_cast &) {}
			return os;
		}

		std::ostream &Error(std::ostream &os)
		{
			return os << Clear << "error: ";
		}
		std::ostream &Warn(std::ostream &os)
		{
			return os << Clear << "warning: ";
		}
	}
}
