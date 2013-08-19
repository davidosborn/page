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
