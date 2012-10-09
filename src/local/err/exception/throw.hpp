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

#ifndef    page_err_exception_throw_hpp
#   define page_err_exception_throw_hpp

#	include <string>
#	include "../../util/sstream.hpp" // make_ostringstream

#	ifdef DEBUG
namespace page
{
	namespace err
	{
		/**
		 * @def THROW
		 *
		 * An alternative to @c throw.  It allows additional information to be
		 * automatically included in the exception, such as the source file and
		 * line number where the exception was thrown.
		 */
		struct Thrower
		{
			Thrower(const char *file, unsigned line) :
				file(file), line(line) {}

			template <typename T> void operator ,(const T &e) const
			{
				MAKE_OSTRINGSTREAM_1(char)//auto ss(util::make_ostringstream<char>());
				ss << file << ':' << line << ": " << e.what();
				throw T(ss.str());
			}

			private:
			const char *file;
			unsigned line;
		};
	}
}
#		define THROW ::page::err::Thrower(__FILE__, __LINE__),
#	else
#		define THROW throw
#	endif

#	include "Exception.hpp"
#	include "tags.hpp" // HACK: included early to solve a dependency cycle
#	include "PlatformException.hpp"
#	include "manip.hpp"
#	include "stdexcept.hpp"
#endif