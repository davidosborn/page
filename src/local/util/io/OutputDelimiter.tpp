/**
 * @section license
 *
 * Copyright (c) 2006-2013 David Osborn
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

namespace page
{
	namespace util
	{
		/*--------------------------+
		| constructors & destructor |
		+--------------------------*/

		template <typename Char, typename CharTraits>
			OutputDelimiter<Char, CharTraits>::OutputDelimiter() {}

		template <typename Char, typename CharTraits>
			OutputDelimiter<Char, CharTraits>::OutputDelimiter(Char c) :
				string(1, c) {}

		template <typename Char, typename CharTraits>
			OutputDelimiter<Char, CharTraits>::OutputDelimiter(const Char *s) :
				string(s) {}

		template <typename Char, typename CharTraits>
			OutputDelimiter<Char, CharTraits>::OutputDelimiter(const String &s) :
				string(s) {}

		/*----------+
		| observers |
		+----------*/

		template <typename Char, typename CharTraits>
			const typename OutputDelimiter<Char, CharTraits>::String &
			OutputDelimiter<Char, CharTraits>::GetString() const
		{
			return string;
		}

		template <typename Char, typename CharTraits>
			OutputDelimiter<Char, CharTraits>::operator String() const
		{
			return string;
		}

		template <typename Char, typename CharTraits>
			OutputDelimiter<Char, CharTraits>::operator bool() const
		{
			return !string.empty();
		}

		/*----------------------+
		| predefined delimiters |
		+----------------------*/

		template <typename Char, typename CharTraits>
			const OutputDelimiter<Char, CharTraits> &
			OutputDelimiter<Char, CharTraits>::GetEmptyDelimiter()
		{
			return OutputDelimiter();
		}

		template <typename Char, typename CharTraits>
			const OutputDelimiter<Char, CharTraits> &
			OutputDelimiter<Char, CharTraits>::GetSpaceDelimiter()
		{
			static OutputDelimiter delimiter(CharTraits::to_char_type(' '));
			return delimiter;
		}

		/*------------------------------+
		| stream insertion & extraction |
		+------------------------------*/

		template <typename Char, typename CharTraits>
			std::basic_ostream<Char, CharTraits> &operator <<(std::basic_ostream<Char, CharTraits> &os, const OutputDelimiter<Char, CharTraits> &delimiter)
		{
			return os << delimiter.GetString();
		}
	}
}