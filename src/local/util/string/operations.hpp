#ifndef    page_local_util_string_operations_hpp
#   define page_local_util_string_operations_hpp

#	include <istream> // basic_istream
#	include <limits> // numeric_limits
#	include <string> // basic_string, char_traits
#	include <utility> // pair

#	include "../io/InputDelimiter.hpp"
#	include "../io/SequenceDeserializationFlags.hpp"

namespace page
{
	namespace util
	{
		/**
		 * @defgroup string
		 * @{
		 */
		/**
		 * @defgroup string-join Join
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
		 * @defgroup string-split Split
		 * @{
		 */
		/**
		 * Splits a string at each occurance of a delimiter, writing the
		 * resulting sub-strings to an output iterator.
		 *
		 * @note This is an overload for @c std::basic_string.
		 */
		template <typename Char, typename CharTraits, typename OutputIterator,
			typename Delimiter = InputDelimiter<Char, CharTraits>>
			void Split(
				const std::basic_string<Char, CharTraits> &,
				OutputIterator,
				typename SequenceDeserializationFlags::Type = SequenceDeserializationFlags::defaultForSpaceDelimiter,
				const Delimiter & = InputDelimiter<Char, CharTraits>::GetSpaceDelimiter(),
				unsigned limit = std::numeric_limits<unsigned>());

		/**
		 * Splits a string at each occurance of a delimiter, writing the
		 * resulting sub-strings to an output iterator.
		 *
		 * @note This is an overload for C strings.
		 */
		template <typename Char, typename OutputIterator,
			typename Delimiter = InputDelimiter<Char>>
			void Split(
				const Char *,
				OutputIterator,
				typename SequenceDeserializationFlags::Type = SequenceDeserializationFlags::defaultForSpaceDelimiter,
				const Delimiter & = InputDelimiter<Char>::GetSpaceDelimiter(),
				unsigned limit = std::numeric_limits<unsigned>());

		/**
		 * Splits a string at the first occurance of a delimiter.
		 *
		 * @return @c std::pair containing the two sub-strings.
		 *
		 * @note This is an overload for @c std::basic_string.
		 */
		template <typename Char, typename CharTraits,
			typename Delimiter = InputDelimiter<Char, CharTraits>>
			std::pair<
				std::basic_string<Char, CharTraits>,
				std::basic_string<Char, CharTraits>>
			Partition(
				const std::basic_string<Char, CharTraits> &,
				typename SequenceDeserializationFlags::Type = SequenceDeserializationFlags::defaultForSpaceDelimiter,
				const Delimiter & = InputDelimiter<Char, CharTraits>::GetSpaceDelimiter());

		/**
		 * Splits a string at the first occurance of a delimiter.
		 *
		 * @return @c std::pair containing the two sub-strings.
		 *
		 * @note This is an overload for C strings.
		 */
		template <typename Char,
			typename Delimiter = InputDelimiter<Char>>
			std::pair<
				std::basic_string<Char>,
				std::basic_string<Char>>
			Partition(
				const Char *,
				typename SequenceDeserializationFlags::Type = SequenceDeserializationFlags::defaultForSpaceDelimiter,
				const Delimiter & = InputDelimiter<Char>::GetSpaceDelimiter());
		///@}

		/**
		 * @defgroup string-trim Trim
		 * @{
		 */
		/**
		 * Removes leading and trailing delimiters from a string.
		 */
		template <typename Char, typename CharTraits,
			typename Delimiter = InputDelimiter<Char, CharTraits>>
			std::basic_string<Char, CharTraits> Trim(
				const std::basic_string<Char, CharTraits> &,
				const Delimiter & = InputDelimiter<Char, CharTraits>::GetSpaceDelimiter());

		/**
		 * Removes leading delimiters from a string.
		 */
		template <typename Char, typename CharTraits,
			typename Delimiter = InputDelimiter<Char, CharTraits>>
			std::basic_string<Char, CharTraits> TrimLeading(
				const std::basic_string<Char, CharTraits> &,
				const Delimiter & = InputDelimiter<Char, CharTraits>::GetSpaceDelimiter());

		/**
		 * Removes trailing delimiters from a string.
		 */
		template <typename Char, typename CharTraits,
			typename Delimiter = InputDelimiter<Char, CharTraits>>
			std::basic_string<Char, CharTraits> TrimTrailing(
				const std::basic_string<Char, CharTraits> &,
				const Delimiter & = InputDelimiter<Char, CharTraits>::GetSpaceDelimiter());
		///@}

		/**
		 * @defgroup string-convert Conversion
		 * @{
		 */
		/**
		 * Converts every character in the string to lowercase.
		 */
		template <typename Char, typename CharTraits>
			std::basic_string<Char, CharTraits>
				ToLower(const std::basic_string<Char, CharTraits> &);

		/**
		 * Converts every character in the string to uppercase.
		 */
		template <typename Char, typename CharTraits>
			std::basic_string<Char, CharTraits>
				ToUpper(const std::basic_string<Char, CharTraits> &);

		/**
		 * Converts various newline formats into the standard format, with each
		 * newline indicated by '\n'.
		 */
		template <typename Char, typename CharTraits>
			std::basic_string<Char, CharTraits> NormalizeNewlines(
				const std::basic_string<Char, CharTraits> &);
		///@}

		/**
		 * @defgroup string-getline Line extraction
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
		 * @defgroup string-compare Comparison
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

#	include "operations.tpp"
#endif
