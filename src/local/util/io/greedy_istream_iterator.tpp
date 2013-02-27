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

#include <algorithm> // find_if_not
#include <cassert> // assert
#include <istream> // basic_istream
#include <iterator> // istreambuf_iterator
#include <sstream> // basic_istringstream
#include <string> // basic_string

namespace page
{
	namespace util
	{
		/*-----------------------+
		| implementation details |
		+-----------------------*/

		namespace detail
		{
			template <typename Char, typename CharTraits, typename T>
				std::basic_istream<Char, CharTraits> &Deserialize(
					std::basic_istream<Char, CharTraits> &is,
					T &value)
			{
				return is >> value;
			}

			template <typename Char, typename CharTraits, typename Allocator>
				std::basic_istream<Char, CharTraits> &Deserialize(
					std::basic_istream<Char, CharTraits> &is,
					std::basic_string<Char, CharTraits, Allocator> &s)
			{
				s.assign(
					std::istreambuf_iterator<Char>(is),
					std::istreambuf_iterator<Char>());
				return is;
			}
		}

		/*--------------------------+
		| constructors & destructor |
		+--------------------------*/

		template <typename T, typename Char, typename CharTraits>
			greedy_istream_iterator<T, Char, CharTraits>::
			greedy_istream_iterator() {}

		template <typename T, typename Char, typename CharTraits>
		template <typename Separator, typename Terminator>
			greedy_istream_iterator<T, Char, CharTraits>::
			greedy_istream_iterator(Stream &is, Separator separator, Terminator terminator) :
				is(&is), separator(separator), terminator(terminator)
		{
			++*this;
		}

		/*--------------------------------+
		| std::istream_iterator semantics |
		+--------------------------------*/

		template <typename T, typename Char, typename CharTraits>
			const T &
			greedy_istream_iterator<T, Char, CharTraits>::
			operator *() const
		{
			return value;
		}

		template <typename T, typename Char, typename CharTraits>
			const T *
			greedy_istream_iterator<T, Char, CharTraits>::
			operator ->() const
		{
			return &value;
		}

		template <typename T, typename Char, typename CharTraits>
			greedy_istream_iterator<T, Char, CharTraits> &
			greedy_istream_iterator<T, Char, CharTraits>::
			operator ++()
		{
			assert(is);

			if (terminated)
			{
				is = nullptr; // become end-of-stream iterator
				return *this;
			}

			typename std::basic_istream<Char, CharTraits>::sentry sentry(*is);
			if (!sentry) return *this;

			// read until next delimiter or end-of-stream
			std::basic_string<Char, CharTraits> s;
			for (;;)
			{
				auto c = is->peek();
				if (!CharTraits::not_eof(c))
				{
					terminated = true;
					break;
				}
				is->ignore();
				auto ch = CharTraits::to_char_type(c);
				if (terminator(ch))
				{
					terminated = true;
					break;
				}
				if (separator(ch))
					break;
				s.push_back(ch);
			}

			// remove trailing whitespace
			if (is->flags() & std::ios_base::skipws)
				s.erase(
					std::find_if_not(s.rbegin(), s.rend(),
						Delimiter::GetSpaceDelimiter()).base(),
					s.end());

			// process string
			if (s.empty())
			{
				if (is->eof())
				{
					is = nullptr; // become end-of-stream iterator
					return *this;
				}
				else value = T();
			}
			else
			{
				// deserialize value from string
				std::basic_istringstream<Char, CharTraits> ss(s);
				if (!detail::Deserialize(ss, value))
					is->setstate(std::ios_base::failbit);
			}

			return *this;
		}

		template <typename T, typename Char, typename CharTraits>
			greedy_istream_iterator<T, Char, CharTraits>
			greedy_istream_iterator<T, Char, CharTraits>::
			operator ++(int)
		{
			greedy_istream_iterator tmp(*this);
			++*this;
			return tmp;
		}

		/*---------------------+
		| relational operators |
		+---------------------*/

		template <typename T, typename Char, typename CharTraits>
			bool greedy_istream_iterator<T, Char, CharTraits>::operator ==(const greedy_istream_iterator<T, Char, CharTraits> &other) const
		{
			return (is && *is) == (other.is && *other.is);
		}

		template <typename T, typename Char, typename CharTraits>
			bool greedy_istream_iterator<T, Char, CharTraits>::operator !=(const greedy_istream_iterator<T, Char, CharTraits> &other) const
		{
			return !(*this == other);
		}
	}
}
