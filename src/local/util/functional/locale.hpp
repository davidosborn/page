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

#ifndef    page_local_util_functional_locale_hpp
#   define page_local_util_functional_locale_hpp

#	include <locale> // isspace, locale, to{lower,upper}

namespace page
{
	namespace util
	{
		/**
		 * @defgroup functional-locale
		 *
		 * Function objects for the localization and character-classification
		 * functions from the @c <locale> and @c <cctype> headers.
		 *
		 * @note Lambda expressions should generally be preferred to these.
		 *
		 * @{
		 */
		template <typename Char>
			struct isnewline_function
		{
			bool operator ()(Char c) const
			{
				switch (c)
				{
					case '\n':
					case '\r': return true;
					default:   return false;
				}
			}
		};
		
		template <typename Char> 
			struct isspace_function
		{
			bool operator ()(Char c) const
			{
				return std::isspace(c, std::locale());
			}
		};
		
		template <typename Char> 
			struct tolower_function
		{
			bool operator ()(Char c) const
			{
				return std::tolower(c, std::locale());
			}
		};
		
		template <typename Char> 
			struct toupper_function
		{
			bool operator ()(Char c) const
			{
				return std::toupper(c, std::locale());
			}
		};
		///@}
	}
}

#endif
