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

#ifndef    page_util_string_hpp
#   define page_util_string_hpp

#	include <istream> // basic_istream
#	include <limits> // numeric_limits
#	include <string> // basic_string, char_traits
#	include <utility> // pair
#	include "serialize/DeserializationDelimiter.hpp"
#	include "serialize/SequenceDeserializationFlags.hpp"

namespace page
{
	namespace util
	{
		/**
		 * @defgroup string String
		 * @{
		 */
		/**
		 * @defgroup string-join String joining
		 * @{
		 */
		/**
		 * Concatenates the variadic arguments into a string.
		 */
		template <
			typename Char       = char,
			typename CharTraits = std::char_traits<Char>,
			typename... Parts>
			std::basic_string<Char, CharTraits> Join(Parts &&...);
		///@}

		/**
		 * @defgroup string-split String splitting
		 * @{
		 */
		/**
		 * Splits a string at each occurance of a delimiter, writing the
		 * resulting sub-strings to an output iterator.
		 *
		 * @note This is an overload for @c std::basic_string.
		 */
		template <typename Char, typename CharTraits, typename OutputIterator,
			typename Delimiter = typename DeserializationDelimiter<Char, CharTraits>::Type>
			void Split(
				const std::basic_string<Char, CharTraits> &,
				OutputIterator,
				typename SequenceDeserializationFlags::Type = SequenceDeserializationFlags::defaultForSpaceDelimiter,
				const Delimiter & = DeserializationDelimiter<Char, CharTraits>::Space(),
				unsigned limit = std::numeric_limits<unsigned>());

		/**
		 * Splits a string at each occurance of a delimiter, writing the
		 * resulting sub-strings to an output iterator.
		 *
		 * @note This is an overload for C strings.
		 */
		template <typename Char, typename OutputIterator,
			typename Delimiter = typename DeserializationDelimiter<Char>::Type>
			void Split(
				const Char *,
				OutputIterator,
				typename SequenceDeserializationFlags::Type = SequenceDeserializationFlags::defaultForSpaceDelimiter,
				const Delimiter & = DeserializationDelimiter<Char>::Space(),
				unsigned limit = std::numeric_limits<unsigned>());

		/**
		 * Splits a string at the first occurance of a delimiter.
		 *
		 * @return @c std::pair containing the two sub-strings.
		 *
		 * @note This is an overload for @c std::basic_string.
		 */
		template <typename Char, typename CharTraits,
			typename Delimiter = typename DeserializationDelimiter<Char, CharTraits>::Type>
			std::pair<
				std::basic_string<Char, CharTraits>,
				std::basic_string<Char, CharTraits>>
			Partition(
				const std::basic_string<Char, CharTraits> &,
				typename SequenceDeserializationFlags::Type = SequenceDeserializationFlags::defaultForSpaceDelimiter,
				const Delimiter & = DeserializationDelimiter<Char, CharTraits>::Space());

		/**
		 * Splits a string at the first occurance of a delimiter.
		 *
		 * @return @c std::pair containing the two sub-strings.
		 *
		 * @note This is an overload for C strings.
		 */
		template <typename Char,
			typename Delimiter = typename DeserializationDelimiter<Char>::Type>
			std::pair<
				std::basic_string<Char>,
				std::basic_string<Char>>
			Partition(
				const Char *,
				typename SequenceDeserializationFlags::Type = SequenceDeserializationFlags::defaultForSpaceDelimiter,
				const Delimiter & = DeserializationDelimiter<Char>::Space());
		///@}

		/**
		 * @defgroup string-trim String trimming
		 * @{
		 */
		/**
		 * Removes leading and trailing delimiters from a string.
		 */
		template <typename Char, typename CharTraits,
			typename Delimiter = typename DeserializationDelimiter<Char, CharTraits>::Type>
			std::basic_string<Char, CharTraits> Trim(
				const std::basic_string<Char, CharTraits> &,
				const Delimiter & = DeserializationDelimiter<Char, CharTraits>::Space());

		/**
		 * Removes leading delimiters from a string.
		 */
		template <typename Char, typename CharTraits,
			typename Delimiter = typename DeserializationDelimiter<Char, CharTraits>::Type>
			std::basic_string<Char, CharTraits> TrimLeading(
				const std::basic_string<Char, CharTraits> &,
				const Delimiter & = DeserializationDelimiter<Char, CharTraits>::Space());

		/**
		 * Removes trailing delimiters from a string.
		 */
		template <typename Char, typename CharTraits,
			typename Delimiter = typename DeserializationDelimiter<Char, CharTraits>::Type>
			std::basic_string<Char, CharTraits> TrimTrailing(
				const std::basic_string<Char, CharTraits> &,
				const Delimiter & = DeserializationDelimiter<Char, CharTraits>::Space());
		///@}

		/**
		 * @defgroup string-convert String conversion
		 * @{
		 */
		/**
		 * Converts various newline formats into the standard format, with each
		 * newline indicated by '\n'.
		 */
		template <typename Char, typename CharTraits>
			std::basic_string<Char, CharTraits> NormalizeNewlines(
				const std::basic_string<Char, CharTraits> &);
		///@}

		/**
		 * @defgroup string-getline String extraction
		 * @{
		 */
		/**
		 * An alternative to @c std::getline with universal newline handling.
		 *
		 * @throw nothrow
		 *
		 * @note Unlike @c std::getline, this function does not set @c failbit
		 *       when an empty string is read.
		 */
		template <typename Char, typename CharTraits>
			std::basic_istream<Char, CharTraits> &getline(
				std::basic_istream<Char, CharTraits> &,
				std::basic_string<Char, CharTraits> &);

		/**
		 * An alternative to @c std::getline.
		 *
		 * @throw nothrow
		 *
		 * @note Unlike @c std::getline, this function does not set @c failbit
		 *       when an empty string is read.
		 */
		template <typename Char, typename CharTraits, typename Delimiter>
			std::basic_istream<Char, CharTraits> &getline(
				std::basic_istream<Char, CharTraits> &,
				std::basic_string<Char, CharTraits> &,
				const Delimiter &delimiter);
		///@}

		/**
		 * @defgroup string-compare String comparison
		 * @{
		 */
		/**
		 * Compares the leading portion of a string with another string.
		 *
		 * @return @c true if the first string starts with the second string.
		 */
		template <typename Char, typename CharTraits>
			bool StartsWith(
				const std::basic_string<Char, CharTraits> &,
				const std::basic_string<Char, CharTraits> &);
		/**
		 * Compares the trailing portion of a string with another string.
		 *
		 * @return @c true if the first string ends with the second string.
		 */
		template <typename Char, typename CharTraits>
			bool EndsWith(
				const std::basic_string<Char, CharTraits> &,
				const std::basic_string<Char, CharTraits> &);
		///@}
		///@}
	}
}

#	include "string.tpp"
#endif
