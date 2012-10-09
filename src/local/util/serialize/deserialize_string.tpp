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

#include <utility> // forward
#include "../../err/exception/throw.hpp" // THROW
#include "../sstream.hpp" // make_istringstream
#include "deserialize.hpp" // Deserialize

namespace page
{
	namespace util
	{
		/**
		 * @weakgroup deserialize-from-string
		 * @{
		 */
		template <typename Char, typename CharTraits, typename Allocator, typename... Args> const std::basic_string<Char, CharTraits, Allocator> &Deserialize(const std::basic_string<Char, CharTraits, Allocator> &s, Args &&... args)
		{
			MAKE_ISTRINGSTREAM_3(Char, CharTraits, Allocator, s)//auto ss(make_istringstream(s));
			if (Deserialize(ss, std::forward<Args>(args)...).fail())
				THROW err::Exception<err::DeserializationTag>();
			return s;
		}
		template <typename Char, typename... Args> const Char *Deserialize(const Char *s, Args &&... args)
		{
			MAKE_ISTRINGSTREAM_1(Char, s)//auto ss(make_istringstream(s));
			if (Deserialize(ss, std::forward<Args>(args)...).fail())
				THROW err::Exception<err::DeserializationTag>();
			return s;
		}
		///@}
	}
}
