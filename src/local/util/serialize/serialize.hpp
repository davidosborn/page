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

#ifndef    page_local_util_serialize_serialize_hpp
#   define page_local_util_serialize_serialize_hpp

#	include <iosfwd> // basic_ostream
#	include <string> // basic_string
#	include <tuple>
#	include <utility> // declval, pair
#	include "../type_traits.hpp" // DEFINE_SFINAE_TYPE_TRAIT_1, ENABLE_IF, is_{insertable,iterator,range,string}
#	include "SerializationDelimiter.hpp"

namespace page
{
	namespace util
	{
		/**
		 * @defgroup serialize-to-stream Serialization to streams
		 * @ingroup  serialize
		 * @{
		 */
		/**
		 * Serialize a value to a stream.
		 *
		 * @throw nothrow Sets @c failbit on error.
		 */
		template <typename Char, typename CharTraits, typename T>
			std::basic_ostream<Char, CharTraits> &Serialize(
				std::basic_ostream<Char, CharTraits> &,
				const T &,
				ENABLE_IF((is_insertable<const T &, std::basic_ostream<Char, CharTraits> &>::value)));
		
		/**
		 * Serialize a string to a stream, where the character encoding of
		 * the string is different from that of the stream.
		 *
		 * @throw nothrow Sets @c failbit on error.
		 */
		template <typename Char, typename CharTraits, typename String>
			std::basic_ostream<Char, CharTraits> &Serialize(
				std::basic_ostream<Char, CharTraits> &,
				const String &,
				ENABLE_IF((
					is_string<String>::value &&
					!is_string_2<String, Char, CharTraits>::value)));
		///@}

		/**
		 * @defgroup serialize-sequence-to-stream Serialization of sequences to streams
		 * @ingroup  serialize-to-stream
		 * @{
		 */
		/**
		 * Serialize a range of values to a stream.
		 *
		 * @throw nothrow Sets @c failbit on error.
		 */
		template <typename Char, typename CharTraits, typename Range,
			typename Delimiter = typename SerializationDelimiter<Char, CharTraits>::Type>
			std::basic_ostream<Char, CharTraits> &Serialize(
				std::basic_ostream<Char, CharTraits> &,
				const Range &,
				const Delimiter & = SerializationDelimiter<Char, CharTraits>::Space(),
				ENABLE_IF((
					is_range<Range>::value &&
					!is_string<Range>::value &&
					!is_insertable<const Range &, std::basic_ostream<Char, CharTraits> &>::value)));

		/**
		 * Serialize a range of values to a stream, where the range is specified
		 * by the @c first and @c last iterators, and where @c last points to
		 * one element past the end of the range.
		 *
		 * @throw nothrow Sets @c failbit on error.
		 */
		template <typename Char, typename CharTraits, typename Iterator,
			typename Delimiter = typename SerializationDelimiter<Char, CharTraits>::Type>
			std::basic_ostream<Char, CharTraits> &Serialize(
				std::basic_ostream<Char, CharTraits> &,
				Iterator first,
				Iterator last,
				const Delimiter & = SerializationDelimiter<Char, CharTraits>::Space(),
				ENABLE_IF(is_iterator<Iterator>::value));
		
		/**
		 * Serialize @c std::tuple to a stream.
		 *
		 * @throw nothrow Sets @c failbit on error.
		 */
		template <typename Char, typename CharTraits, typename... Types,
			typename Delimiter = typename SerializationDelimiter<Char, CharTraits>::Type>
			std::basic_ostream<Char, CharTraits> &Serialize(
				std::basic_ostream<Char, CharTraits> &,
				const std::tuple<Types...> &,
				const Delimiter & = SerializationDelimiter<Char, CharTraits>::Space());

		/**
		 * Serialize @c std::pair to a stream.
		 *
		 * @throw nothrow Sets @c failbit on error.
		 */
		template <typename Char, typename CharTraits, typename First, typename Second,
			typename Delimiter = typename SerializationDelimiter<Char, CharTraits>::Type>
			std::basic_ostream<Char, CharTraits> &Serialize(
				std::basic_ostream<Char, CharTraits> &,
				const std::pair<First, Second> &,
				const Delimiter & = SerializationDelimiter<Char, CharTraits>::Space());
		///@}
		
		/**
		 * @addtogroup serialize-to-stream
		 * @{
		 */
		/**
		 * Serialize to a stream rvalue.  This is useful for working with
		 * @c std::basic_ostringstream.
		 */
		template <typename Char, typename CharTraits, typename... Args>
			std::basic_ostream<Char, CharTraits> &Serialize(
				std::basic_ostream<Char, CharTraits> &&,
				Args &&...);
		///@}
		

		/**
		 * @addtogroup serialize
		 * @{
		 */
		/**
		 * A type trait for detecting whether a type will be serialized as a
		 * sequence, in which case @c Serialize will take a delimiter, or as a
		 * value, in which case @c Serialize will not take any additional
		 * arguments.
		 */
		DEFINE_SFINAE_TYPE_TRAIT_1(is_serializable_as_sequence,
			decltype(
				Serialize(
					std::declval<std::ostream &>(),
					std::declval<const T &>(),
					SerializationDelimiter<char>::None()),
				std::declval<void>()))
		///@}
	}
}

#	include "serialize.tpp"
#endif
