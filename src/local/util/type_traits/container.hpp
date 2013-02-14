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

#ifndef    page_local_util_type_traits_container_hpp
#   define page_local_util_type_traits_container_hpp

#	include <cstddef> // {nullptr,ptrdiff,size}_t
#	include <iterator> // begin, end
#	include <type_traits>
#	include <utility> // declval

#	include "sfinae.hpp"

namespace page
{
	namespace util
	{
		/**
		 * Detects whether a type is a range, which includes any type for which
		 * @c std::begin(x) and @c std::end(x) are valid function calls.
		 */
		DEFINE_SFINAE_TYPE_TRAIT_1(is_range,
			decltype(
				std::begin(std::declval<T>()),
				std::end  (std::declval<T>()), std::declval<void>()))

		/**
		 * Determines the element type of a range.
		 */
		template <typename T>
			struct range_element_type
		{
			typedef
				typename std::decay<
					decltype(*std::begin(std::declval<T>()))
					>::type type;
		};

		/**
		 * Detects whether const is propogated to the elements of a range.
		 *
		 * @note This type trait will only produce the correct result when the
		 *       elements are non-const.
		 *
		 * @note If you're using this type trait to limit a range argument to
		 *       non-const when const is propogated for that range, you should
		 *       also use @c std::is_const to ensure the argument is non-const.
		 */
		DEFINE_SFINAE_TYPE_TRAIT_1(is_const_propogated,
			typename std::enable_if<
				std::is_const<
					typename std::remove_reference<
						decltype(*std::begin(std::declval<const T>()))
						>::type
					>::value
				>::type)

		/**
		 * Detects whether a type is compatible with @c std::insert_iterator.
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
	}
}

#endif
