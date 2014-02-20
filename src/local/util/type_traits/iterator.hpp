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

#ifndef    page_local_util_type_traits_iterator_hpp
#   define page_local_util_type_traits_iterator_hpp

#	include <iterator> // iterator_traits
#	include <type_traits>

#	include "sfinae.hpp"

namespace page { namespace util
{
	DEFINE_SFINAE_TYPE_TRAIT_1(has_container_type,    typename T::container_type)
	DEFINE_SFINAE_TYPE_TRAIT_1(has_iterator_category, typename T::iterator_category)

	/**
	 * Detects whether a type is an iterator, which includes raw pointers but
	 * excludes smart pointers.
	 */
	template <typename T>
		struct is_iterator :
			std::integral_constant<bool,
				has_iterator_category<T>::value ||
				std::is_pointer<T>::value> {};

	/**
	 * @defgroup output_iterator_value_type
	 * @{
	 */
	namespace detail
	{
		// level 2
		template <typename T, bool = has_container_type<T>::value>
			struct output_iterator_value_type_impl_2
		{
			typedef typename T::container_type::value_type type;
		};
		template <typename T>
			struct output_iterator_value_type_impl_2<T, false>
		{
			typedef typename std::iterator_traits<T>::value_type type;
		};

		// level 1
		template <typename T, bool = is_iterator<T>::value>
			struct output_iterator_value_type_impl
		{
			using type = typename output_iterator_value_type_impl_2<T>::type;
		};
		template <typename T>
			struct output_iterator_value_type_impl<T, false>
		{
			using type = void; // T is not an iterator
		};
	}

	/**
	 * Determines the @b natural @c value_type of an output iterator.
	 *
	 * @note If the iterator is not an output iterator, this metafunction will
	 *       return the same type as @c std::iterator_traits<T>::value_type.
	 */
	template <typename T>
		struct output_iterator_value_type
	{
		using type = typename detail::output_iterator_value_type_impl<T>::type;
	};
	///@}
}}

#endif
