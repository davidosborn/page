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

#ifndef    page_local_util_io_OutputDelimiter_hpp
#   define page_local_util_io_OutputDelimiter_hpp

#	include <string> // basic_string, char_traits

namespace page
{
	namespace util
	{
		/**
		 * A class representing an output delimiter.
		 */
		template <typename Char, typename CharTraits = std::char_traits<Char>>
			class OutputDelimiter
		{
			public:
			typedef std::basic_string<Char, CharTraits> String;

			/*-------------+
			| constructors |
			+-------------*/

			/**
			 * Creates an empty delimiter.
			 */
			OutputDelimiter();

			/**
			 * Creates a delimiter consisting of a single character.
			 */
			OutputDelimiter(Char);

			/**
			 * Creates a delimiter consisting of a string.
			 */
			OutputDelimiter(const Char *);

			/**
			 * Creates a delimiter consisting of a string.
			 */
			OutputDelimiter(const String &);

			/*----------+
			| observers |
			+----------*/

			/**
			 * @return @copydoc string
			 */
			const String &GetString() const;

			/**
			 * Convert the delimiter to a string.
			 */
			operator String() const;

			/**
			 * @return @c true if the delimiter is not empty.
			 */
			explicit operator bool() const;

			/*----------------------+
			| predefined delimiters |
			+----------------------*/

			/**
			 * @return The predefined empty-delimiter, which is empty.
			 */
			static const OutputDelimiter &GetEmptyDelimiter();

			/**
			 * @return The predefined whitespace-delimiter, which consists of a
			 *         single space character.
			 */
			static const OutputDelimiter &GetSpaceDelimiter();

			/*-------------+
			| data members |
			+-------------*/

			private:
			/**
			 * The string which is used as the delimiter.
			 */
			String string;
		};

		/*----------------------------+
		| stream insertion/extraction |
		+----------------------------*/

		/**
		 * Writes the delimiter to a stream.
		 */
		template <typename Char, typename CharTraits>
			std::basic_ostream<Char, CharTraits> &operator <<(std::basic_ostream<Char, CharTraits> &, const OutputDelimiter<Char, CharTraits> &);
	}
}

#	include "OutputDelimiter.tpp"
#endif
