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

#ifndef    page_local_util_type_traits_range_hpp
#   define page_local_util_type_traits_range_hpp

#	include <iterator> // begin, end
#	include <type_traits>
#	include <utility> // declval

#	include "sfinae.hpp"

namespace page { namespace util
{
	/**
	 * Detects whether a type is a range, which includes any type for which @c
	 * std::begin(x) and @c std::end(x) are valid function calls.
	 */
	DEFINE_SFINAE_TYPE_TRAIT_1(is_range,
		decltype(
			std::begin(std::declval<T>()),
			std::end  (std::declval<T>()),
			std::declval<void>()))

	/**
	 * A collection of type traits for a range, comparable to @c
	 * std::iterator_traits.
	 */
	template <typename Range,
		typename Iterator = typename std::decay<decltype(std::begin(std::declval<Range>()))>::type>
		struct range_traits : std::iterator_traits<Iterator>
	{
		using iterator        = Iterator;
		using iterator_traits = std::iterator_traits<iterator>;
	};

	/**
	 * Detects whether @c const is propogated to the elements of a range.
	 *
	 * @note This type trait will only produce the correct result when the
	 *       elements are not themselves @c const.
	 */
	DEFINE_SFINAE_TYPE_TRAIT_1(is_const_propogated,
		typename std::enable_if<
			std::is_const<
				typename range_traits<const T>::value_type
				>::value
			>::type)
}}

#endif
