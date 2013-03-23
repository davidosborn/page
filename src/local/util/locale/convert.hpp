/**
 * @section license
 *
 * Copyright (c) 2006-2013 David Osborn
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

#ifndef    page_local_util_locale_convert_hpp
#   define page_local_util_locale_convert_hpp

#	include <string> // allocator, basic_string, char_traits

#	include "CharEncoding.hpp"

namespace page
{
	namespace util
	{
		/**
		 * Converts a string to a different character encoding.
		 *
		 * Characters that cannot be represented in the new encoding are
		 * dropped.
		 *
		 * @throw err::Exception<err::UtilModuleTag, err::ConversionTag>
		 */
		template <
			CharEncoding   ToCharEncoding,
			CharEncoding FromCharEncoding = CharEncoding::defaultForType,
			typename       ToChar       = typename GetCharEncodingType<ToCharEncoding>::type,
			typename       ToCharTraits = std::char_traits<ToChar>,
			typename       ToAllocator  = std::allocator<ToChar>,
			typename     FromChar,
			typename     FromCharTraits = std::char_traits<FromChar>,
			typename     FromAllocator  = std::allocator<FromChar>>
				std::basic_string<ToChar, ToCharTraits, ToAllocator>
				Convert(const std::basic_string<FromChar, FromCharTraits, FromAllocator> &);

		/**
		 * @copydoc Convert
		 */
		template <
			CharEncoding   ToCharEncoding,
			CharEncoding FromCharEncoding = CharEncoding::defaultForType,
			typename       ToChar       = typename GetCharEncodingType<ToCharEncoding>::type,
			typename       ToCharTraits = std::char_traits<ToChar>,
			typename       ToAllocator  = std::allocator<ToChar>,
			typename     FromChar,
			typename     FromCharTraits = std::char_traits<FromChar>,
			typename     FromAllocator  = std::allocator<FromChar>>
				std::basic_string<ToChar, ToCharTraits, ToAllocator>
				Convert(const FromChar *);
	}
}

#	include "convert.tpp"
#endif
