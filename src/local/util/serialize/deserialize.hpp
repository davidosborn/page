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

#ifndef    page_local_util_serialize_deserialize_hpp
#   define page_local_util_serialize_deserialize_hpp

#	include <iosfwd> // basic_istream
#	include <limits> // numeric_limits
#	include <string> // basic_string
#	include <tuple>
#	include <type_traits> // is_pointer
#	include <utility> // pair
#	include "../type_traits.hpp" // ENABLE_IF, is_{compatible_with_insert_iterator,const_propogated,extractable,iterator,range,string}
#	include "DeserializationDelimiter.hpp"
#	include "SequenceDeserializationFlags.hpp"

namespace page
{
	namespace util
	{
		/**
		 * @defgroup deserialize-from-stream Deserialization from streams
		 * @ingroup  deserialize
		 * @{
		 */
		/**
		 * Deserialize a non-string value from a stream.
		 *
		 * @throw nothrow Sets @c failbit on error.
		 */
		template <typename Char, typename CharTraits, typename T>
			std::basic_istream<Char, CharTraits> &Deserialize(
				std::basic_istream<Char, CharTraits> &,
				T &,
				ENABLE_IF((
					is_extractable<T &, std::basic_istream<Char, CharTraits> &>::value &&
					(!is_iterator<T>::value || std::is_pointer<T>::value) &&
					!is_string<T>::value)));

		/**
		 * Deserialize a string from a stream.
		 *
		 * @note Characters from the stream will be added to the string until
		 *       either EOF is reached or a terminating character is read.
		 *
		 * @throw nothrow Sets @c failbit on error.
		 */
		template <typename Char, typename CharTraits, typename String,
			typename Terminator = typename DeserializationDelimiter<Char>::Type>
			std::basic_istream<Char, CharTraits> &Deserialize(
				std::basic_istream<Char, CharTraits> &,
				String &,
				const Terminator & = DeserializationDelimiter<Char>::None(),
				ENABLE_IF((is_string_2<String, Char, CharTraits>::value)));

		/**
		 * Deserialize a string from a stream, where the character encoding of
		 * the string is different from that of the stream.
		 *
		 * @note Characters from the stream will be added to the string until
		 *       either EOF is reached or a terminating character is read.
		 *
		 * @throw nothrow Sets @c failbit on error.
		 */
		template <typename Char, typename CharTraits, typename String,
			typename Terminator = typename DeserializationDelimiter<Char>::Type>
			std::basic_istream<Char, CharTraits> &Deserialize(
				std::basic_istream<Char, CharTraits> &,
				String &,
				const Terminator & = DeserializationDelimiter<Char>::None(),
				ENABLE_IF((
					is_string<String>::value &&
					!is_string_2<String, Char, CharTraits>::value)));
		///@}

		/**
		 * @defgroup deserialize-sequence-from-stream Deserialization of sequences from streams
		 * @ingroup  deserialize-from-stream
		 * @{
		 */
		/**
		 * Deserialize an unlimited sequence of values from a stream, where the
		 * values are written to a sequence container.
		 *
		 * @note The sequence container will be cleared before deserialization
		 *       begins.
		 * @note The values will be deserialized in sequence until either EOF is
		 *       reached, a terminating character is read, or the number of
		 *       values deserialized reaches @c limit.
		 *
		 * @throw nothrow Sets @c failbit on error.
		 */
		template <typename Char, typename CharTraits, typename Sequence,
			typename Delimiter  = typename DeserializationDelimiter<Char>::Type,
			typename Terminator = typename DeserializationDelimiter<Char>::Type>
			std::basic_istream<Char, CharTraits> &Deserialize(
				std::basic_istream<Char, CharTraits> &,
				Sequence &,
				typename SequenceDeserializationFlags::Type = SequenceDeserializationFlags::defaultForSpaceDelimiter,
				const Delimiter  & = DeserializationDelimiter<Char>::Space(),
				const Terminator & = DeserializationDelimiter<Char>::None(),
				unsigned limit = std::numeric_limits<unsigned>::max(),
				ENABLE_IF((
					util::is_range<Sequence>::value &&
					!util::is_string<Sequence>::value &&
					!util::is_extractable<Sequence &, std::basic_istream<Char, CharTraits> &>::value &&
					util::is_compatible_with_insert_iterator<Sequence>::value)));

		/**
		 * Deserialize an unlimited sequence of values from a stream, where the
		 * values are written to an output iterator.
		 *
		 * @note The values will be deserialized in sequence until either EOF is
		 *       reached, a terminating character is read, or the number of
		 *       values deserialized reaches @c limit.
		 *
		 * @throw nothrow Sets @c failbit on error.
		 */
		template <typename Char, typename CharTraits, typename OutputIterator,
			typename Delimiter  = typename DeserializationDelimiter<Char>::Type,
			typename Terminator = typename DeserializationDelimiter<Char>::Type>
			std::basic_istream<Char, CharTraits> &Deserialize(
				std::basic_istream<Char, CharTraits> &,
				OutputIterator,
				typename SequenceDeserializationFlags::Type = SequenceDeserializationFlags::defaultForSpaceDelimiter,
				const Delimiter  & = DeserializationDelimiter<Char>::Space(),
				const Terminator & = DeserializationDelimiter<Char>::None(),
				unsigned limit = std::numeric_limits<unsigned>::max(),
				ENABLE_IF(
					is_iterator<OutputIterator>::value &&
					!std::is_pointer<OutputIterator>::value));

		/**
		 * Deserialize a range of values from a stream, where the constness of
		 * the range is not propogated to its elements.
		 *
		 * @note The values will be deserialized in sequence until either EOF is
		 *       reached or the entire range has been deserialized.
		 * @note If EOF is reached before the entire range has been
		 *       deserialized, the @c failbit will be set on the stream.
		 *
		 * @throw nothrow Sets @c failbit on error.
		 */
		template <typename Char, typename CharTraits, typename Range,
			typename Delimiter     = typename DeserializationDelimiter<Char>::Type,
			typename LastDelimiter = typename DeserializationDelimiter<Char>::Type>
			std::basic_istream<Char, CharTraits> &Deserialize(
				std::basic_istream<Char, CharTraits> &,
				const Range &,
				typename SequenceDeserializationFlags::Type = SequenceDeserializationFlags::defaultForSpaceDelimiter,
				const     Delimiter & = DeserializationDelimiter<Char>::Space(),
				const LastDelimiter & = DeserializationDelimiter<Char>::None(),
				ENABLE_IF((
					util::is_range<Range>::value &&
					!util::is_const_propogated<Range>::value &&
					!util::is_string<Range>::value &&
					!util::is_extractable<Range &, std::basic_istream<Char, CharTraits> &>::value &&
					!util::is_compatible_with_insert_iterator<Range>::value)));

		/**
		 * Deserialize a range of values from a stream, where the constness of
		 * the range is propogated to its elements.
		 *
		 * @note The values will be deserialized in sequence until either EOF is
		 *       reached or the entire range has been deserialized.
		 * @note If EOF is reached before the entire range has been
		 *       deserialized, the @c failbit will be set on the stream.
		 *
		 * @throw nothrow Sets @c failbit on error.
		 */
		template <typename Char, typename CharTraits, typename Range,
			typename Delimiter     = typename DeserializationDelimiter<Char>::Type,
			typename LastDelimiter = typename DeserializationDelimiter<Char>::Type>
			std::basic_istream<Char, CharTraits> &Deserialize(
				std::basic_istream<Char, CharTraits> &,
				Range &,
				typename SequenceDeserializationFlags::Type = SequenceDeserializationFlags::defaultForSpaceDelimiter,
				const     Delimiter & = DeserializationDelimiter<Char>::Space(),
				const LastDelimiter & = DeserializationDelimiter<Char>::None(),
				ENABLE_IF((
					util::is_range<Range>::value &&
					util::is_const_propogated<Range>::value && !std::is_const<Range>::value &&
					!util::is_string<Range>::value &&
					!util::is_extractable<Range &, std::basic_istream<Char, CharTraits> &>::value &&
					!util::is_compatible_with_insert_iterator<Range>::value)));

		/**
		 * Deserialize a range of values from a stream, where the range is
		 * specified by the @c first and @c last iterators, and where @c last
		 * points to one element past the end of the range.
		 *
		 * @note The values will be deserialized in sequence until either EOF is
		 *       reached or the entire range has been deserialized.
		 * @note If EOF is reached before the entire range has been
		 *       deserialized, the @c failbit will be set on the stream.
		 *
		 * @throw nothrow Sets @c failbit on error.
		 */
		template <typename Char, typename CharTraits, typename Iterator,
			typename Delimiter     = typename DeserializationDelimiter<Char>::Type,
			typename LastDelimiter = typename DeserializationDelimiter<Char>::Type>
			std::basic_istream<Char, CharTraits> &Deserialize(
				std::basic_istream<Char, CharTraits> &,
				Iterator first,
				Iterator last,
				typename SequenceDeserializationFlags::Type = SequenceDeserializationFlags::defaultForSpaceDelimiter,
				const     Delimiter & = DeserializationDelimiter<Char>::Space(),
				const LastDelimiter & = DeserializationDelimiter<Char>::None(),
				ENABLE_IF(is_iterator<Iterator>::value));

		/**
		 * Deserialize @c std::tuple from a stream, where the tuple is an
		 * lvalue reference.
		 *
		 * @note The tuple elements will be deserialized in sequence until
		 *       either EOF is reached or the entire tuple has been
		 *       deserialized.
		 * @note If EOF is reached before the entire tuple has been
		 *       deserialized, the @c failbit will be set on the stream.
		 *
		 * @throw nothrow Sets @c failbit on error.
		 */
		template <typename Char, typename CharTraits, typename... Types,
			typename Delimiter     = typename DeserializationDelimiter<Char>::Type,
			typename LastDelimiter = typename DeserializationDelimiter<Char>::Type>
			std::basic_istream<Char, CharTraits> &Deserialize(
				std::basic_istream<Char, CharTraits> &,
				std::tuple<Types...> &,
				typename SequenceDeserializationFlags::Type = SequenceDeserializationFlags::defaultForSpaceDelimiter,
				const     Delimiter & = DeserializationDelimiter<Char>::Space(),
				const LastDelimiter & = DeserializationDelimiter<Char>::None());
		
		/**
		 * Deserialize @c std::tuple from a stream, where the tuple is an rvalue
		 * reference.
		 *
		 * @note The tuple elements will be deserialized in sequence until
		 *       either EOF is reached or the entire tuple has been
		 *       deserialized.
		 * @note If EOF is reached before the entire tuple has been
		 *       deserialized, the @c failbit will be set on the stream.
		 *
		 * @throw nothrow Sets @c failbit on error.
		 */
		template <typename Char, typename CharTraits, typename... Types,
			typename Delimiter     = typename DeserializationDelimiter<Char>::Type,
			typename LastDelimiter = typename DeserializationDelimiter<Char>::Type>
			std::basic_istream<Char, CharTraits> &Deserialize(
				std::basic_istream<Char, CharTraits> &,
				std::tuple<Types...> &&,
				typename SequenceDeserializationFlags::Type = SequenceDeserializationFlags::defaultForSpaceDelimiter,
				const     Delimiter & = DeserializationDelimiter<Char>::Space(),
				const LastDelimiter & = DeserializationDelimiter<Char>::None());

		/**
		 * Deserialize @c std::pair from a stream, where the pair is an lvalue
		 * reference.
		 *
		 * @throw nothrow Sets @c failbit on error.
		 */
		template <typename Char, typename CharTraits, typename First, typename Second,
			typename Delimiter     = typename DeserializationDelimiter<Char>::Type,
			typename LastDelimiter = typename DeserializationDelimiter<Char>::Type>
			std::basic_istream<Char, CharTraits> &Deserialize(
				std::basic_istream<Char, CharTraits> &,
				std::pair<First, Second> &,
				typename SequenceDeserializationFlags::Type = SequenceDeserializationFlags::defaultForSpaceDelimiter,
				const     Delimiter & = DeserializationDelimiter<Char>::Space(),
				const LastDelimiter & = DeserializationDelimiter<Char>::None());

		/**
		 * Deserialize @c std::pair from a stream, where the pair is an rvalue
		 * reference.
		 *
		 * @throw nothrow Sets @c failbit on error.
		 */
		template <typename Char, typename CharTraits, typename First, typename Second,
			typename Delimiter     = typename DeserializationDelimiter<Char>::Type,
			typename LastDelimiter = typename DeserializationDelimiter<Char>::Type>
			std::basic_istream<Char, CharTraits> &Deserialize(
				std::basic_istream<Char, CharTraits> &,
				std::pair<First, Second> &&,
				typename SequenceDeserializationFlags::Type = SequenceDeserializationFlags::defaultForSpaceDelimiter,
				const     Delimiter & = DeserializationDelimiter<Char>::Space(),
				const LastDelimiter & = DeserializationDelimiter<Char>::None());
		///@}

		/**
		 * @addtogroup deserialize-from-stream
		 * @{
		 */
		/**
		 * Deserialize from a stream rvalue.  This is useful for working with
		 * @c std::basic_istringstream.
		 */
		template <typename Char, typename CharTraits, typename... Args>
			std::basic_istream<Char, CharTraits> &Deserialize(
				std::basic_istream<Char, CharTraits> &&,
				Args &&...);
		///@}

		/**
		 * @addtogroup deserialize
		 * @{
		 */
		/**
		 * A type trait for detecting whether a type will be deserialized as a
		 * sequence, in which case @c Deserialize will take a delimiter, or as a
		 * value, in which case @c Deserialize will not take any additional
		 * arguments.
		 */
		DEFINE_SFINAE_TYPE_TRAIT_1(is_deserializable_as_sequence,
			decltype(
				Deserialize(
					std::declval<std::istream &>(),
					std::declval<T &>(),
					SequenceDeserializationFlags::none,
					DeserializationDelimiter<char>::None()),
				std::declval<void>()))
		///@}
	}
}

#	include "deserialize.tpp"
#endif
