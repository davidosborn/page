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

#include <cstring> // strlen

#include "Stream.hpp"

namespace page
{
	namespace log
	{
		/*----------+
		| interface |
		+----------*/

		void Stream::Write(char c)
		{
			DoWrite(std::string(1, c));
		}

		void Stream::Write(const char *s, unsigned n)
		{
			DoWrite(std::string(s, s + n));
		}

		void Stream::Write(const std::string &s)
		{
			DoWrite(s);
		}

		void Stream::Write(const char *s)
		{
			DoWrite(s);
		}

		void Stream::Flush()
		{
			DoFlush();
		}

		void Stream::Clear()
		{
			DoClear();
		}

		/*------------------+
		| virtual functions |
		+------------------*/

		void Stream::DoFlush() {}
		void Stream::DoClear() {}

		////////////////////////////////////////////////////////////////////////

		/*--------------------------+
		| std::streambuf conversion |
		+--------------------------*/

		std::streambuf &Stream::streambuf()
		{
			return *this;
		}

		const std::streambuf &Stream::streambuf() const
		{
			return *this;
		}

		/*------------------------------+
		| std::streambuf implementation |
		+------------------------------*/

		int Stream::sync()
		{
			try
			{
				Flush();
			}
			catch (...)
			{
				return -1;
			}
			return 0;
		}

		std::streamsize Stream::xsputn(const char *s, std::streamsize n)
		{
			try
			{
				Write(s, n);
			}
			catch (...)
			{
				return 0;
			}
			return n;
		}

		std::streambuf::int_type Stream::overflow(std::streambuf::int_type c)
		{
			if (traits_type::eq_int_type(c, traits_type::eof()))
				return traits_type::not_eof(c);

			try
			{
				Write(traits_type::to_char_type(c));
			}
			catch (...)
			{
				return traits_type::eof();
			}
			return c;
		}
	}
}
