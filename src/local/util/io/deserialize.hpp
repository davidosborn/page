#ifndef    page_local_util_io_deserialize_hpp
#   define page_local_util_io_deserialize_hpp

#	include <iosfwd> // basic_istream
#	include <limits> // numeric_limits
#	include <string> // basic_string
#	include <tuple>
#	include <type_traits> // is_pointer
#	include <utility> // pair

#	include "../locale/CharEncoding.hpp"
#	include "../type_traits/container.hpp" // is_compatible_with_insert_iterator
#	include "../type_traits/iterator.hpp" // is_iterator
#	include "../type_traits/range.hpp" // is_{const_propogated,range}
#	include "../type_traits/sfinae.hpp" // DEFINE_SFINAE_TYPE_TRAIT, ENABLE_IF
#	include "../type_traits/stream.hpp" // is_extractable
#	include "../type_traits/string.hpp" // is_string
#	include "InputDelimiter.hpp"
#	include "SequenceDeserializationFlags.hpp"

namespace page
{
	namespace util
	{
		/**
		 * @defgroup deserialize Deserialization
		 * @{
		 */
		/**
		 * @defgroup deserialize-stream From stream
		 * Functions for deserializing from streams.
		 * @{
		 */
		/**
		 * @defgroup deserialize-stream-value To value
		 * Functions for deserializing values (as opposed to sequences).
		 * @{
		 */
		/**
		 * Deserializes a non-string value from a stream.
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
		 * @defgroup deserialize-stream-value-string String
		 * Functions for deserializing strings.
		 * @{
		 */
		/**
		 * Deserializes a string from a stream.
		 *
		 * @note Characters from the stream will be added to the string until
		 *       either EOF is reached or a terminating character is read.
		 *
		 * @throw nothrow Sets @c failbit on error.
		 */
		template <typename Char, typename CharTraits, typename String,
			typename Terminator = const InputDelimiter<Char, CharTraits> &>
			std::basic_istream<Char, CharTraits> &Deserialize(
				std::basic_istream<Char, CharTraits> &,
				String &,
				Terminator = InputDelimiter<Char, CharTraits>::GetEmptyDelimiter(),
				ENABLE_IF((is_string_2<String, Char, CharTraits>::value)));

		/**
		 * Deserializes a string from a stream, where the character encoding of
		 * the string is different from that of the stream.
		 *
		 * @note Characters from the stream will be added to the string until
		 *       either EOF is reached or a terminating character is read.
		 *
		 * @throw nothrow Sets @c failbit on error.
		 */
		template <
			CharEncoding   ToCharEncoding = CharEncoding::defaultForType,
			CharEncoding FromCharEncoding = CharEncoding::defaultForType,
			typename Char, typename CharTraits, typename String,
			typename Terminator = const InputDelimiter<Char, CharTraits> &>
			std::basic_istream<Char, CharTraits> &Deserialize(
				std::basic_istream<Char, CharTraits> &,
				String &,
				Terminator = InputDelimiter<Char, CharTraits>::GetEmptyDelimiter(),
				ENABLE_IF((
					is_string<String>::value &&
					!is_string_2<String, Char, CharTraits>::value)));
		///@}
		///@}

		/**
		 * @defgroup deserialize-stream-sequence To sequence
		 * Functions for deserializing sequences.
		 * @{
		 */
		/**
		 * @defgroup deserialize-stream-sequence-homogeneous Homogeneous
		 * Functions for deserializing homogeneous sequences.
		 * @{
		 */
		/**
		 * @defgroup deserialize-stream-sequence-homogeneous-unlimited Unlimited
		 * Functions for deserializing unlimited-length homogeneous-sequences.
		 * @{
		 */
		/**
		 * Deserializes an unlimited sequence of values from a stream, where the
		 * values are written to a "sequence container" (ISO/IEC N3242 23.2.3).
		 *
		 * @note The sequence container will be cleared before deserialization
		 *       begins.
		 *
		 * @note The values will be deserialized in sequence until either EOF is
		 *       reached, a terminating character is read, or the number of
		 *       values deserialized reaches @c limit.
		 *
		 * @throw nothrow Sets @c failbit on error.
		 */
		template <typename Char, typename CharTraits, typename Container,
			typename Separator  = const InputDelimiter<Char, CharTraits> &,
			typename Terminator = const InputDelimiter<Char, CharTraits> &>
			std::basic_istream<Char, CharTraits> &Deserialize(
				std::basic_istream<Char, CharTraits> &,
				Container &,
				typename SequenceDeserializationFlags::Type = SequenceDeserializationFlags::defaultForSpaceDelimiter,
				Separator  = InputDelimiter<Char, CharTraits>::GetSpaceDelimiter(),
				Terminator = InputDelimiter<Char, CharTraits>::GetEmptyDelimiter(),
				unsigned limit = std::numeric_limits<unsigned>::max(),
				ENABLE_IF((
					util::is_range<Container>::value &&
					!util::is_string<Container>::value &&
					!util::is_extractable<Container &, std::basic_istream<Char, CharTraits> &>::value &&
					util::is_compatible_with_insert_iterator<Container>::value)));

		/**
		 * Deserializes an unlimited sequence of values from a stream, where the
		 * values are written to an "output iterator" (ISO/IEC N3242 24.2.4).
		 *
		 * @note The values will be deserialized in sequence until either EOF is
		 *       reached, a terminating character is read, or the number of
		 *       values deserialized reaches @c limit.
		 *
		 * @throw nothrow Sets @c failbit on error.
		 */
		template <typename Char, typename CharTraits, typename OutputIterator,
			typename Separator  = const InputDelimiter<Char, CharTraits> &,
			typename Terminator = const InputDelimiter<Char, CharTraits> &>
			std::basic_istream<Char, CharTraits> &Deserialize(
				std::basic_istream<Char, CharTraits> &,
				OutputIterator,
				typename SequenceDeserializationFlags::Type = SequenceDeserializationFlags::defaultForSpaceDelimiter,
				Separator  = InputDelimiter<Char, CharTraits>::GetSpaceDelimiter(),
				Terminator = InputDelimiter<Char, CharTraits>::GetEmptyDelimiter(),
				unsigned limit = std::numeric_limits<unsigned>::max(),
				ENABLE_IF((
					is_iterator<OutputIterator>::value &&
					!std::is_pointer<OutputIterator>::value)));
		///@}

		/**
		 * @defgroup deserialize-stream-sequence-homogeneous-limited Limited
		 * Functions for deserializing limited-length homogeneous-sequences.
		 * @{
		 */
		/**
		 * Deserializes a limited sequence of values from a stream, where the
		 * values are written to a "container" (ISO/IEC N3242 23.2.1) whose
		 * constness is not propogated to its elements.
		 *
		 * @note This overload is chosen only when the overload for a "sequence
		 *       container" (ISO/IEC N3242 23.2.3) is not available.
		 *
		 * @note The values will be deserialized in sequence until either EOF is
		 *       reached or the entire range has been deserialized.
		 *
		 * @note If EOF is reached before the entire range has been
		 *       deserialized, the @c failbit will be set on the stream.
		 *
		 * @throw nothrow Sets @c failbit on error.
		 */
		template <typename Char, typename CharTraits, typename Container,
			typename Separator     = const InputDelimiter<Char, CharTraits> &,
			typename LastSeparator = const InputDelimiter<Char, CharTraits> &>
			std::basic_istream<Char, CharTraits> &Deserialize(
				std::basic_istream<Char, CharTraits> &,
				const Container &,
				typename SequenceDeserializationFlags::Type = SequenceDeserializationFlags::defaultForSpaceDelimiter,
				Separator     = InputDelimiter<Char, CharTraits>::GetSpaceDelimiter(),
				LastSeparator = InputDelimiter<Char, CharTraits>::GetEmptyDelimiter(),
				ENABLE_IF((
					util::is_range<Container>::value &&
					!util::is_string<Container>::value &&
					!util::is_extractable<Container &, std::basic_istream<Char, CharTraits> &>::value &&
					!util::is_compatible_with_insert_iterator<Container>::value &&
					!util::is_const_propogated<Container>::value)));

		/**
		 * Deserializes a limited sequence of values from a stream, where the
		 * values are written to a "container" (ISO/IEC N3242 23.2.1) whose
		 * constness is propogated to its elements.
		 *
		 * @note This overload is chosen only when the overload for a "sequence
		 *       container" (ISO/IEC N3242 23.2.3) is not available.
		 *
		 * @note The values will be deserialized in sequence until either EOF is
		 *       reached or the entire range has been deserialized.
		 *
		 * @note If EOF is reached before the entire range has been
		 *       deserialized, the @c failbit will be set on the stream.
		 *
		 * @throw nothrow Sets @c failbit on error.
		 */
		template <typename Char, typename CharTraits, typename Container,
			typename Separator     = const InputDelimiter<Char, CharTraits> &,
			typename LastSeparator = const InputDelimiter<Char, CharTraits> &>
			std::basic_istream<Char, CharTraits> &Deserialize(
				std::basic_istream<Char, CharTraits> &,
				Container &,
				typename SequenceDeserializationFlags::Type = SequenceDeserializationFlags::defaultForSpaceDelimiter,
				Separator     = InputDelimiter<Char, CharTraits>::GetSpaceDelimiter(),
				LastSeparator = InputDelimiter<Char, CharTraits>::GetEmptyDelimiter(),
				ENABLE_IF((
					util::is_range<Container>::value &&
					!util::is_string<Container>::value &&
					!util::is_extractable<Container &, std::basic_istream<Char, CharTraits> &>::value &&
					!util::is_compatible_with_insert_iterator<Container>::value &&
					util::is_const_propogated<Container>::value && !std::is_const<Container>::value)));

		/**
		 * Deserializes a limited sequence of values from a stream, where the
		 * values are written to a range defined by two iterators, @c first
		 * pointing to the first element and @c last pointing to one element
		 * after the last element.
		 *
		 * @note The values will be deserialized in sequence until either EOF is
		 *       reached or the entire range has been deserialized.
		 *
		 * @note If EOF is reached before the entire range has been
		 *       deserialized, the @c failbit will be set on the stream.
		 *
		 * @throw nothrow Sets @c failbit on error.
		 */
		template <typename Char, typename CharTraits, typename InputIterator,
			typename Separator     = const InputDelimiter<Char, CharTraits> &,
			typename LastSeparator = const InputDelimiter<Char, CharTraits> &>
			std::basic_istream<Char, CharTraits> &Deserialize(
				std::basic_istream<Char, CharTraits> &,
				InputIterator first,
				InputIterator last,
				typename SequenceDeserializationFlags::Type = SequenceDeserializationFlags::defaultForSpaceDelimiter,
				Separator     = InputDelimiter<Char, CharTraits>::GetSpaceDelimiter(),
				LastSeparator = InputDelimiter<Char, CharTraits>::GetEmptyDelimiter(),
				ENABLE_IF((is_iterator<InputIterator>::value)));
		///@}
		///@}

		/**
		 * @defgroup deserialize-stream-sequence-heterogeneous Heterogeneous
		 * Functions for deserializing heterogeneous sequences.
		 * @{
		 */
		/**
		 * Deserializes a @c std::tuple from a stream.
		 *
		 * @note The tuple elements will be deserialized in sequence until
		 *       either EOF is reached or the entire tuple has been
		 *       deserialized.
		 *
		 * @note If EOF is reached before the entire tuple has been
		 *       deserialized, the @c failbit will be set on the stream.
		 *
		 * @throw nothrow Sets @c failbit on error.
		 */
		template <typename Char, typename CharTraits, typename... Types,
			typename Separator     = const InputDelimiter<Char, CharTraits> &,
			typename LastSeparator = const InputDelimiter<Char, CharTraits> &>
			std::basic_istream<Char, CharTraits> &Deserialize(
				std::basic_istream<Char, CharTraits> &,
				std::tuple<Types...> &,
				typename SequenceDeserializationFlags::Type = SequenceDeserializationFlags::defaultForSpaceDelimiter,
				Separator     = InputDelimiter<Char, CharTraits>::GetSpaceDelimiter(),
				LastSeparator = InputDelimiter<Char, CharTraits>::GetEmptyDelimiter());

		/**
		 * Deserializes a @c std::pair from a stream.
		 *
		 * @throw nothrow Sets @c failbit on error.
		 */
		template <typename Char, typename CharTraits, typename First, typename Second,
			typename Separator     = const InputDelimiter<Char, CharTraits> &,
			typename LastSeparator = const InputDelimiter<Char, CharTraits> &>
			std::basic_istream<Char, CharTraits> &Deserialize(
				std::basic_istream<Char, CharTraits> &,
				std::pair<First, Second> &,
				typename SequenceDeserializationFlags::Type = SequenceDeserializationFlags::defaultForSpaceDelimiter,
				Separator     = InputDelimiter<Char, CharTraits>::GetSpaceDelimiter(),
				LastSeparator = InputDelimiter<Char, CharTraits>::GetEmptyDelimiter());
		///@}
		///@}

		/**
		 * Deserializes from a stream rvalue-reference.
		 *
		 * @throw err::Exception<err::UtilModuleTag, err::SerializationTag>
		 */
		template <typename Char, typename CharTraits, typename... Args>
			void Deserialize(std::basic_istream<Char, CharTraits> &&, Args &&...);
		///@}

		/**
		 * @defgroup deserialize-string From string
		 * Functions for deserializing from strings.
		 * @{
		 */
		/**
		 * Deserializes from @c std::basic_string by forwarding to the stream
		 * deserialization functions.
		 *
		 * @throw err::Exception<err::UtilModuleTag, err::SerializationTag>
		 */
		template <typename Char, typename CharTraits, typename Allocator, typename... Args>
			const std::basic_string<Char, CharTraits, Allocator> &Deserialize(
				const std::basic_string<Char, CharTraits, Allocator> &, Args &&...);

		/**
		 * Deserializes from a string literal by forwarding to the stream
		 * deserialization functions.
		 *
		 * @throw err::Exception<err::UtilModuleTag, err::SerializationTag>
		 */
		template <typename Char, typename... Args>
			const Char *Deserialize(const Char *, Args &&...);
		///@}

		/**
		 * @defgroup deserialize-typetraits Type traits
		 * Type traits related to deserialization.
		 * @{
		 */
		/**
		 * A type trait for detecting whether a type will be deserialized as a
		 * sequence, in which case @c Deserialize will take some flags and a
		 * delimiter, or as a value, in which case @c Deserialize will not take
		 * any additional arguments.
		 */
		DEFINE_SFINAE_TYPE_TRAIT_1(is_deserializable_as_sequence,
			decltype(
				Deserialize(
					std::declval<std::istream &>(),
					std::declval<T &>(),
					SequenceDeserializationFlags::none,
					InputDelimiter<char>()),
				std::declval<void>()))
		///@}
		///@}
	}
}

#	include "deserialize.tpp"
#endif
