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

#ifndef    page_local_util_type_traits_container_hpp
#   define page_local_util_type_traits_container_hpp

#	include <type_traits>
#	include <utility> // declval

#	include "sfinae.hpp"

namespace page { namespace util
{
	/**
	 * Detects whether a container is compatible with @c std::insert_iterator.
	 *
	 * Compatibility depends on whether the expression @c a.insert(p,t) is
	 * valid, which is one of the requirements for sequence and associative
	 * containers (ISO/IEC N3242 23.2.3).
	 */
	DEFINE_SFINAE_TYPE_TRAIT_1(is_compatible_with_insert_iterator,
		decltype(
			std::declval<T>().insert(
				// HACK: GCC doesn't accept const_iterator
				std::declval<typename T::iterator>(),
				std::declval<typename T::value_type>()),
			std::declval<void>()))
}}

#endif
