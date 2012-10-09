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

#ifndef    page_util_serialize_serialize_string_hpp
#   define page_util_serialize_serialize_string_hpp

#	include <ios> // ios_base
#	include <string> // basic_string
#	include <type_traits> // is_base_of, remove_reference
#	include "../pp.hpp" // COMMA
#	include "../type_traits.hpp" // ENABLE_IF

namespace page
{
	namespace util
	{
		/**
		 * @defgroup serialize-to-string Serialization to @c std::basic_string
		 * @ingroup  serialize
		 * @{
		 */
		/**
		 * Serializes to @c std::basic_string by forwarding to the stream
		 * serialization functions.
		 *
		 * @note This function will serialize to a @c char string unless you
		 *       explicitly specify the character type.
		 */
		template <
			typename Char       = char,
			typename CharTraits = std::char_traits<Char>,
			typename Allocator  = std::allocator<Char>,
			typename Arg,
			typename... Args>
			auto Serialize(Arg &&, Args &&...) ->
				ENABLE_IF_RETURN(
					!(std::is_base_of<std::ios_base, typename std::remove_reference<Arg>::type>::value),
					std::basic_string<Char COMMA CharTraits COMMA Allocator>);
		///@}
	}
}

#	include "serialize_string.tpp"
#endif
