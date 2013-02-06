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

#ifndef    page_local_util_io_InputDelimiter_hpp
#   define page_local_util_io_InputDelimiter_hpp

#	include <functional> // function
#	include <string> // char_traits

#	include "../type_traits.hpp" // ENABLE_IF, is_range

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

			/*--------------------------+
			| constructors & destructor |
			+--------------------------*/

			InputDelimiter();
			InputDelimiter(Char);
			InputDelimiter(const Char *);
			InputDelimiter(const Predicate &);

			/**
			 * Create a delimiter from a sequence of characters, which will
			 * match any character in the sequence.
			 */
			template <typename T>
				InputDelimiter(const T &, ENABLE_IF(is_range<T>::value));

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
			 * @return @c true if the delimiter's predicate is callable.
			 */
			explicit operator bool() const;

			/*----------------------+
			| predefined delimiters |
			+----------------------*/

			/**
			 * @return The predefined whitespace delimiter, which matches any
			 * whitespace character.
			 */
			static const InputDelimiter &GetSpaceDelimiter();

			/*-----------------+
			| member variables |
			+-----------------*/

			private:
			/**
			 * A function which determines whether a character is a delimiter.
			 */
			Predicate predicate = nullptr;
		};
	}
}

#	include "InputDelimiter.tpp"
#endif
