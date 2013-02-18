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

#ifndef    page_local_util_type_traits_pointer_hpp
#   define page_local_util_type_traits_pointer_hpp

#	include <type_traits>

namespace page
{
	namespace util
	{
		/**
		 * Removes one level of indirection from a type.
		 */
		template <typename T> struct remove_indirection
		{
			typedef decltype(*std::declval<T>())                    reference;
			typedef typename std::remove_reference<reference>::type type;
			typedef typename std::remove_cv<type>::type             value_type;
		};

		/**
		 * @defgroup remove_member_pointer
		 * @{
		 */
		namespace detail
		{
			template <typename T>
				struct remove_member_pointer_impl
			{
				typedef T type;
			};
			template <typename T, typename U>
				struct remove_member_pointer_impl<T U::*>
			{
				typedef T type;
			};
		}

		/**
		 * Removes the member pointer from a type, leaving the type of the member
		 * pointed to.
		 */
		template <typename T>
			struct remove_member_pointer :
				detail::remove_member_pointer_impl<T> {};
		///@}
	}
}

#endif
