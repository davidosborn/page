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

#include <algorithm> // find
#include <iterator> // begin, end
#include "../string_convert.hpp" // Convert

namespace page
{
	namespace util
	{
		/**
		 * @weakgroup serialize-delimiter-normalize
		 * @{
		 */
		template <typename Char, typename CharTraits>
			typename SerializationDelimiter<Char, CharTraits>::Type
			SerializationDelimiter<Char, CharTraits>::Normalize(
				const Type &delimiter)
		{
			return delimiter;
		}
		template <typename Char, typename CharTraits>
			typename SerializationDelimiter<Char, CharTraits>::Type
			SerializationDelimiter<Char, CharTraits>::Normalize(
				Char c)
		{
			return Type(1, c);
		}
		template <typename Char, typename CharTraits>
			template <typename Range>
			typename SerializationDelimiter<Char, CharTraits>::Type
			SerializationDelimiter<Char, CharTraits>::Normalize(
				const Range &range,
				ENABLE_IF_IMPL(
					is_range<Range>::value &&
					!is_string<Range>::value))
		{
			return Normalize(
				std::basic_string<typename range_element_type<Range>::type>(
					std::begin(range), std::end(range)));
		}
		template <typename Char, typename CharTraits>
			template <typename String>
			typename SerializationDelimiter<Char, CharTraits>::Type
			SerializationDelimiter<Char, CharTraits>::Normalize(
				const String &s,
				ENABLE_IF_IMPL((
					is_string<String>::value &&
					!is_string_2<String, Char, CharTraits>::value)))
		{
			return Convert<Char, CharTraits>(s);
		}
		template <typename Char, typename CharTraits>
			template <typename String>
			typename SerializationDelimiter<Char, CharTraits>::Type
			SerializationDelimiter<Char, CharTraits>::Normalize(
				const String &s,
				ENABLE_IF_IMPL((
					!is_range<String>::value &&
					is_convertible_to_string<String>::value)))
		{
			return Normalize(std::basic_string<
				typename string_literal_char_type<String>::type>(s));
		}
		template <typename Char, typename CharTraits>
			template <typename Other>
			typename SerializationDelimiter<Char, CharTraits>::Type
			SerializationDelimiter<Char, CharTraits>::Normalize(
				const Other &other,
				ENABLE_IF_IMPL((
					!is_range<Other>::value &&
					!std::is_convertible<Other, Type>::value &&
					std::is_explicitly_convertible<Other, Type>::value)))
		{
			return Type(other);
		}
		///@}
	}
}
