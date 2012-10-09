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

#include <algorithm> // min
#include <cstring> // memcpy
#include "../../err/exception/catch.hpp" // CATCH_TAGS
#include "../../err/exception/throw.hpp" // THROW
#include "BufferFilter.hpp"

namespace page
{
	namespace log
	{
		BufferFilter::BufferFilter()
		{
			setp(data, data + sizeof data);
		}
		BufferFilter::BufferFilter(const std::shared_ptr<Stream> &link) :
			Filter(link)
		{
			setp(data, data + sizeof data);
		}

		void BufferFilter::Put(char c)
		{
			if (traits_type::eq_int_type(sputc(c), traits_type::eof()))
				THROW err::EndOfStreamException<>();
		}
		void BufferFilter::Put(const char *s, unsigned n)
		{
			if (sputn(s, n) < n) THROW err::StreamWriteException<>();
		}

		void BufferFilter::Sync()
		{
			Filter::Put(pbase(), pptr() - pbase());
			setp(pbase(), epptr());
			Filter::Sync();
		}

		std::streamsize BufferFilter::xsputn(const char *s, std::streamsize n)
		{
			std::streamsize w = n;
			try
			{
				while (n)
				{
					std::streamsize w = std::min(epptr() - pptr(), n);
					std::memcpy(pptr(), s, w);
					pbump(w); s += w; n -= w;
					if (pptr() == epptr())
					{
						Filter::Put(pbase(), pptr() - pbase());
						setp(pbase(), epptr());
					}
				}
			}
			CATCH_TAGS(err::StreamTag, err::WriteTag) {}
			return w - n;
		}
		std::streambuf::int_type BufferFilter::overflow(std::streambuf::int_type c)
		{
			try
			{
				Filter::Put(pbase(), pptr() - pbase());
				setp(pbase(), epptr());
				if (traits_type::eq_int_type(c, traits_type::eof()))
					return traits_type::not_eof(c);
				Filter::Put(traits_type::to_char_type(c));
			}
			CATCH_TAGS(err::StreamTag, err::WriteTag)
			{
				return traits_type::eof();
			}
			return c;
		}
	}
}
