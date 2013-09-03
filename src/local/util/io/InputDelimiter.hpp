#ifndef    page_local_util_io_InputDelimiter_hpp
#   define page_local_util_io_InputDelimiter_hpp

#	include <functional> // function
#	include <string> // char_traits

#	include "../type_traits/iterator.hpp" // is_iterator
#	include "../type_traits/range.hpp" // is_range
#	include "../type_traits/sfinae.hpp" // ENABLE_IF

namespace page
{
	namespace util
	{
		/**
		 * A class representing an input delimiter.
		 */
		template <typename Char, typename CharTraits = std::char_traits<Char>>
			class InputDelimiter
		{
			public:
			typedef std::function<bool (Char)> Predicate;

			/*-------------+
			| constructors |
			+-------------*/

			/**
			 * Creates a delimiter that will never match.
			 */
			InputDelimiter();

			/**
			 * Creates a delimiter that will match the specified character.
			 */
			InputDelimiter(Char);

			/**
			 * Creates a delimiter that will match any of the specified
			 * characters.
			 */
			InputDelimiter(const Char *);

			/**
			 * Create a delimiter that will match any of the specified
			 * characters.
			 */
			template <typename InputRange>
				InputDelimiter(InputRange,
					ENABLE_IF(is_range<InputRange>::value));

			/**
			 * Create a delimiter that will match any of the specified
			 * characters.
			 */
			template <typename InputIterator>
				InputDelimiter(
					InputIterator first,
					InputIterator last,
					ENABLE_IF(is_iterator<InputIterator>::value));

			/**
			 * Creates a delimiter that will match according to a predicate.
			 *
			 * @note It is impossible to determine whether the provided
			 *       predicate is empty, so we assume that it is not.
			 */
			InputDelimiter(const Predicate &);

			/*----------+
			| observers |
			+----------*/

			/**
			 * @return @copydoc predicate
			 */
			const Predicate &GetPredicate() const;

			/**
			 * Call the delimiter's predicate.
			 */
			bool operator()(Char c) const;

			/**
			 * @return @c true if the delimiter is not empty.
			 */
			explicit operator bool() const;

			/*----------------------+
			| predefined delimiters |
			+----------------------*/

			/**
			 * @return The predefined empty-delimiter, which doesn't match any
			 *         characters.
			 */
			static const InputDelimiter &GetEmptyDelimiter();

			/**
			 * @return The predefined whitespace-delimiter, which matches any
			 *         whitespace character.
			 */
			static const InputDelimiter &GetSpaceDelimiter();

			/*-------------+
			| data members |
			+-------------*/

			private:
			/**
			 * A function which determines whether a character is a delimiter.
			 */
			Predicate predicate;
		};
	}
}

#	include "InputDelimiter.tpp"
#endif
