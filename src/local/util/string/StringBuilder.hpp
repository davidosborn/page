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

#ifndef    page_local_util_string_StringBuilder_hpp
#   define page_local_util_string_StringBuilder_hpp

#	include <sstream> // basic_{ostringstream,string}

namespace page { namespace util
{
	/**
	 * An alternative to std::basic_ostringstream, which allows you to write
	 * one-liners, since the insertion operator returns the complete type.
	 */
	template <
		typename Char,
		typename CharTraits = std::char_traits<Char>>
			class BasicStringBuilder
	{
		public:
		typedef std::basic_string<Char, CharTraits> String;

		template <typename T>
			BasicStringBuilder &operator <<(T &&);

		operator String() const;

		private:
		std::basic_ostringstream<Char, CharTraits> ss;
	};

	typedef BasicStringBuilder<char>    StringBuilder;
	typedef BasicStringBuilder<wchar_t> WideStringBuilder;
}}

#	include "StringBuilder.tpp"
#endif
