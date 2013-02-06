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

#include <sstream> // basic_ostringstream
#include <utility> // forward

#include "../../err/Exception.hpp"
#include "serialize.hpp" // Serialize

namespace page
{
	namespace util
	{
		/**
		 * @weakgroup serialize-to-string
		 * @{
		 */
		template <typename Char, typename CharTraits, typename Allocator, typename Arg, typename... Args> auto Serialize(Arg &&arg, Args &&... args) -> ENABLE_IF_RETURN(!(std::is_base_of<std::ios_base, typename std::remove_reference<Arg>::type>::value), std::basic_string<Char COMMA CharTraits COMMA Allocator>)
		{
			std::basic_ostringstream<Char, CharTraits, Allocator> ss;
			if (Serialize(ss, std::forward<Arg>(arg), std::forward<Args>(args)...).fail())
				THROW((err::Exception<err::UtilModuleTag, err::SerializationTag>()))
			return ss.str();
		}
		///@}
	}
}
