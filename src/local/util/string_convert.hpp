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

#ifndef    page_local_util_string_convert_hpp
#   define page_local_util_string_convert_hpp

#	include <string> // allocator, basic_string, char_traits
#	include "Flags.hpp"

namespace page
{
	namespace util
	{
		/**
		 * A collection of bit-flags for modifying the behaviour of the Convert
		 * function.
		 */
		struct ConvertFlags : Flags
		{
			enum
			{
				/**
				 * Indicates that the string has a variable-width encoding, such
				 * as UTF-8 or UTF-16, rather than a fixed-width encoding, such
				 * as ASCII or UCS-2.
				 *
				 * @verbatim
				 *
				 * Type     | Fixed-width Encoding | Variable-width Encoding
				 * ---------+----------------------+------------------------
				 * char     | ASCII-7              | UTF-8
				 * wchar_t  | UCS-2/UCS-4          | UTF-16/UTF-32
				 * char16_t | UCS-2                | UTF-16
				 * char32_t | UCS-4                | UTF-32
				 *
				 * @endverbatim
				 */
				variableWidth = 0x01
			};
		};

		/**
		 * Convert a string to a different character encoding, with the same
		 * flags for input and output.
		 *
		 * @throw err::Exception<err::ConversionTag>
		 */
		template <
			typename ToChar,
			typename ToCharTraits = std::char_traits<ToChar>,
			typename ToAllocator  = std::allocator<ToChar>,
			typename FromChar,
			typename FromCharTraits,
			typename FromAllocator>
			std::basic_string<ToChar, ToCharTraits, ToAllocator> Convert(
				const std::basic_string<FromChar, FromCharTraits, FromAllocator> &,
				typename ConvertFlags::Type = ConvertFlags::none);

		/**
		 * Convert a string to a different character encoding.
		 *
		 * @throw err::Exception<err::ConversionTag>
		 */
		template <
			typename ToChar,
			typename ToCharTraits = std::char_traits<ToChar>,
			typename ToAllocator  = std::allocator<ToChar>,
			typename FromChar,
			typename FromCharTraits,
			typename FromAllocator>
			std::basic_string<ToChar, ToCharTraits, ToAllocator> Convert(
				const std::basic_string<FromChar, FromCharTraits, FromAllocator> &,
				typename ConvertFlags::Type fromFlags,
				typename ConvertFlags::Type   toFlags);
	}
}

#	include "string_convert.tpp"
#endif
