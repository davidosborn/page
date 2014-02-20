/**
 * @copyright
 *
 * Copyright (c) 2006-2014 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the same place
 *    and form as other copyright, license, and disclaimer information.
 *
 * 3. Redistributions in binary form must also include an acknowledgement in the
 *    same place and form as other acknowledgements (such as the credits),
 *    similar in substance to the following:
 *
 *       Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

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
