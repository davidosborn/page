#ifndef    page_local_util_io_serialize_hpp
#   define page_local_util_io_serialize_hpp

#	include <ios> // ios_base
#	include <iosfwd> // basic_ostream
#	include <string> // basic_string
#	include <tuple>
#	include <type_traits> // is_base_of, remove_reference
#	include <utility> // declval, pair

#	include "../cpp.hpp" // COMMA
#	include "../locale/CharEncoding.hpp"
#	include "../type_traits/iterator.hpp" // is_iterator
#	include "../type_traits/range.hpp" // is_range
#	include "../type_traits/sfinae.hpp" // DEFINE_SFINAE_TYPE_TRAIT, ENABLE_IF
#	include "../type_traits/stream.hpp" // is_insertable
#	include "../type_traits/string.hpp" // is_string
#	include "OutputDelimiter.hpp"

namespace page
{
	namespace util
	{
		/**
		 * @defgroup serialize Serialization
		 * @{
		 */
		/**
		 * @defgroup serialize-stream To stream
		 * Functions for serializing to streams.
		 * @{
		 */
		/**
		 * @defgroup serialize-stream-value From value
		 * Functions for serializing values (as opposed to sequences).
		 * @{
		 */
		/**
		 * Serializes a value to a stream.
		 *
		 * @throw nothrow Sets @c failbit on error.
		 */
		template <typename Char, typename CharTraits, typename T>
			std::basic_ostream<Char, CharTraits> &Serialize(
				std::basic_ostream<Char, CharTraits> &,
				const T &,
				ENABLE_IF((is_insertable<const T &, std::basic_ostream<Char, CharTraits> &>::value)));

		/**
		 * @defgroup serialize-stream-value-string String
		 * Functions for serializing strings.
		 * @{
		 */
		/**
		 * Serializes a string to a stream, where the character encoding of
		 * the string is different from that of the stream.
		 *
		 * @throw nothrow Sets @c failbit on error.
		 */
		template <
			CharEncoding   ToCharEncoding = CharEncoding::defaultForType,
			CharEncoding FromCharEncoding = CharEncoding::defaultForType,
			typename Char, typename CharTraits, typename String>
			std::basic_ostream<Char, CharTraits> &Serialize(
				std::basic_ostream<Char, CharTraits> &,
				const String &,
				ENABLE_IF((
					is_string<String>::value &&
					!is_string_2<String, Char, CharTraits>::value)));
		///@}
		///@}

		/**
		 * @defgroup serialize-stream-sequence From sequence
		 * Functions for serializing sequences.
		 * @{
		 */
		/**
		 * @defgroup serialize-stream-sequence-homogeneous Homogeneous
		 * Functions for serializing homogeneous sequences.
		 * @{
		 */
		/**
		 * Serializes a range of values to a stream.
		 *
		 * @throw nothrow Sets @c failbit on error.
		 */
		template <typename Char, typename CharTraits, typename InputRange,
			typename Separator = const OutputDelimiter<Char, CharTraits> &>
			std::basic_ostream<Char, CharTraits> &Serialize(
				std::basic_ostream<Char, CharTraits> &,
				const InputRange &,
				Separator = OutputDelimiter<Char, CharTraits>::GetSpaceDelimiter(),
				ENABLE_IF((
					is_range<InputRange>::value &&
					!is_string<InputRange>::value &&
					!is_insertable<InputRange, std::basic_ostream<Char, CharTraits> &>::value)));

		/**
		 * Serializes a range of values to a stream, where the range is
		 * defined by two iterators, @c first pointing to the first element and
		 * @c last pointing to one element after the last element.
		 *
		 * @throw nothrow Sets @c failbit on error.
		 */
		template <typename Char, typename CharTraits, typename InputIterator,
			typename Separator = const OutputDelimiter<Char, CharTraits> &>
			std::basic_ostream<Char, CharTraits> &Serialize(
				std::basic_ostream<Char, CharTraits> &,
				InputIterator first,
				InputIterator last,
				Separator = OutputDelimiter<Char, CharTraits>::GetSpaceDelimiter(),
				ENABLE_IF((is_iterator<InputIterator>::value)));
		///@}

		/**
		 * @defgroup serialize-stream-sequence-heterogeneous Heterogeneous
		 * Functions for serializing heterogeneous sequences.
		 * @{
		 */
		/**
		 * Serializes a @c std::tuple to a stream.
		 *
		 * @throw nothrow Sets @c failbit on error.
		 */
		template <typename Char, typename CharTraits, typename... Types,
			typename Separator = const OutputDelimiter<Char, CharTraits> &>
			std::basic_ostream<Char, CharTraits> &Serialize(
				std::basic_ostream<Char, CharTraits> &,
				const std::tuple<Types...> &,
				Separator = OutputDelimiter<Char, CharTraits>::GetSpaceDelimiter());

		/**
		 * Serializes a @c std::pair to a stream.
		 *
		 * @throw nothrow Sets @c failbit on error.
		 */
		template <typename Char, typename CharTraits, typename First, typename Second,
			typename Separator = const OutputDelimiter<Char, CharTraits> &>
			std::basic_ostream<Char, CharTraits> &Serialize(
				std::basic_ostream<Char, CharTraits> &,
				const std::pair<First, Second> &,
				Separator = OutputDelimiter<Char, CharTraits>::GetSpaceDelimiter());
		///@}
		///@}

		/**
		 * Serializes to a stream rvalue-reference.
		 */
		/*template <typename Char, typename CharTraits, typename... Args>
			void Serialize(std::basic_ostream<Char, CharTraits> &&, Args &&...);*/
		///@}

		/**
		 * @defgroup serialize-string To string
		 * Functions for serializing to strings.
		 * @{
		 */
		/**
		 * Serializes to @c std::basic_string by forwarding to the stream
		 * serialization functions.
		 *
		 * @note This function will serialize to a @c char string unless you
		 *       explicitly specify the character type.
		 *
		 * @throw err::Exception<err::UtilModuleTag, err::SerializationTag>
		 */
		template <
			typename Char       = char,
			typename CharTraits = std::char_traits<Char>,
			typename Allocator  = std::allocator<Char>,
			typename Arg, typename... Args>
			auto Serialize(Arg &&, Args &&...) ->
				ENABLE_IF_RETURN(
					!(std::is_base_of<std::ios_base, typename std::remove_reference<Arg>::type>::value),
					std::basic_string<Char COMMA CharTraits COMMA Allocator>);
		///@}

		/**
		 * @defgroup serialize-typetraits Type traits
		 * Type traits related to serialization.
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
					OutputDelimiter<char>()),
				std::declval<void>()))
		///@}
		///@}
	}
}

#	include "serialize.tpp"
#endif
