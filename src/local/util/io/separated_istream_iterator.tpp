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

#include <cassert> // assert
#include <istream> // basic_istream
#include <string> // basic_string

namespace page
{
	namespace util
	{
		/*--------------------------+
		| constructors & destructor |
		+--------------------------*/

		template <typename T, typename Char, typename CharTraits>
			separated_istream_iterator<T, Char, CharTraits>::
			separated_istream_iterator() {}

		template <typename T, typename Char, typename CharTraits>
			template <typename Separator>
			separated_istream_iterator<T, Char, CharTraits>::
			separated_istream_iterator(istream_type &is, Separator separator) :
				is(&is), separator(separator)
		{
			is >> value;
		}

		/*--------------------------------+
		| std::istream_iterator semantics |
		+--------------------------------*/

		template <typename T, typename Char, typename CharTraits>
			const T &
			separated_istream_iterator<T, Char, CharTraits>::
			operator *() const
		{
			return value;
		}

		template <typename T, typename Char, typename CharTraits>
			const T *
			separated_istream_iterator<T, Char, CharTraits>::
			operator ->() const
		{
			return &value;
		}

		template <typename T, typename Char, typename CharTraits>
			separated_istream_iterator<T, Char, CharTraits> &
			separated_istream_iterator<T, Char, CharTraits>::
			operator ++()
		{
			assert(is);

			if (separator)
			{
				Char c;
				*is >> c;
				if (!separator(c))
					is->setstate(std::ios_base::failbit);
			}

			*is >> value;
			return *this;
		}

		template <typename T, typename Char, typename CharTraits>
			separated_istream_iterator<T, Char, CharTraits>
			separated_istream_iterator<T, Char, CharTraits>::
			operator ++(int)
		{
			separated_istream_iterator tmp(*this);
			++*this;
			return tmp;
		}

		/*---------------------+
		| relational operators |
		+---------------------*/

		template <typename T, typename Char, typename CharTraits>
			bool separated_istream_iterator<T, Char, CharTraits>::operator ==(const separated_istream_iterator<T, Char, CharTraits> &other) const
		{
			return (is && *is) == (other.is && *other.is);
		}

		template <typename T, typename Char, typename CharTraits>
			bool separated_istream_iterator<T, Char, CharTraits>::operator !=(const separated_istream_iterator<T, Char, CharTraits> &other) const
		{
			return !(*this == other);
		}
	}
}
