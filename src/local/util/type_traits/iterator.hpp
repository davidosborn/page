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

#ifndef    page_local_util_type_traits_iterator_hpp
#   define page_local_util_type_traits_iterator_hpp

#	include <iterator> // iterator_traits
#	include <type_traits>

#	include "sfinae.hpp"

namespace page
{
	namespace util
	{
		DEFINE_SFINAE_TYPE_TRAIT_1(has_container_type,    typename T::container_type)
		DEFINE_SFINAE_TYPE_TRAIT_1(has_iterator_category, typename T::iterator_category)
		DEFINE_SFINAE_TYPE_TRAIT_1(has_value_type,        typename T::value_type)

		/**
		 * Detects whether a type is an iterator (including a raw pointer).
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
				typedef typename output_iterator_value_type_impl_2<T>::type type;
			};
			template <typename T>
				struct output_iterator_value_type_impl<T, false>
			{
				typedef void type; // T is not an iterator
			};
		}
		
		/**
		 * Like @c std::iterator_traits::value_type, but for output iterators.
		 * Determines the @b natural @c value_type of the output iterator.
		 *
		 * @note If the iterator is not an output iterator, this class will
		 *       return the same type as @c std::iterator_traits<T>::value_type.
		 */
		template <typename T>
			struct output_iterator_value_type
		{
			typedef typename detail::output_iterator_value_type_impl<T>::type type;
		};
		///@}

		/**
		 * @defgroup is_iterator_over
		 * @{
		 */
		namespace detail
		{
			template <typename T, typename, bool = is_iterator<T>::value>
				struct is_iterator_over_impl;
			template <typename T, typename U>
				struct is_iterator_over_impl<T, U, true> :
					std::integral_constant<bool,
						std::is_same<U,
							typename output_iterator_value_type<T>::type>::value> {};
			template <typename T, typename U>
				struct is_iterator_over_impl<T, U, false> :
					std::false_type {};
		}
		
		/**
		 * Detects whether a type is both an iterator and an iterator over a
		 * specific type.
		 *
		 * @deprecated
		 */
		template <typename T, typename U> struct is_iterator_over :
			detail::is_iterator_over_impl<T, U> {};
		///@}
	}
}

#endif
