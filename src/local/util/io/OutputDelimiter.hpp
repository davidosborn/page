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

			/*--------------------------+
			| constructors & destructor |
			+--------------------------*/

			OutputDelimiter();
			OutputDelimiter(Char);
			OutputDelimiter(const Char *);
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
			 * @return @c true if the delimiter's string is not empty.
			 */
			explicit operator bool() const;

			/*----------------------+
			| predefined delimiters |
			+----------------------*/

			/**
			 * @return The predefined whitespace delimiter, which contains a
			 * single space character.
			 */
			static const OutputDelimiter &GetSpaceDelimiter();

			/*-----------------+
			| member variables |
			+-----------------*/

			private:
			/**
			 * The string which is used as the delimiter.
			 */
			String string;
		};
	}
}

#	include "OutputDelimiter.tpp"
#endif
