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

#ifndef    page_local_util_type_traits_sign_hpp
#   define page_local_util_type_traits_sign_hpp

#	include <type_traits>

#	include "integer.hpp"

namespace page
{
	namespace util
	{
		/**
		 * @defgroup make_signed
		 * @{
		 */
		namespace detail
		{
			template <typename T>
				struct make_signed_impl
			{
				static_assert(
					!is_standard_integer_type<T>::value &&
					!is_extended_integer_type<T>::value ||
					std::is_signed<T>::value,
					"failed to make integer type signed");

				typedef T type;
			};
			template <> struct make_signed_impl<unsigned char>      { typedef signed char type; };
			template <> struct make_signed_impl<unsigned short>     { typedef short       type; };
			template <> struct make_signed_impl<unsigned int>       { typedef int         type; };
			template <> struct make_signed_impl<unsigned long>      { typedef long        type; };
			template <> struct make_signed_impl<unsigned long long> { typedef long long   type; };
		}

		/**
		 * An alternative to @c std::make_signed with simplified behaviour.
		 *
		 * @note @c std::make_signed does not work in GCC 4.6.
		 */
		template <typename T>
			struct make_signed :
				detail::make_signed_impl<T> {};
		///@}

		/**
		 * @defgroup make_unsigned
		 * @{
		 */
		namespace detail
		{
			template <typename T>
				struct make_unsigned_impl
			{
				static_assert(
					!is_standard_integer_type<T>::value &&
					!is_extended_integer_type<T>::value ||
					std::is_unsigned<T>::value, "failed to make integer type unsigned");

				typedef T type;
			};
			template <> struct make_unsigned_impl<signed char> { typedef unsigned char      type; };
			template <> struct make_unsigned_impl<short>       { typedef unsigned short     type; };
			template <> struct make_unsigned_impl<int>         { typedef unsigned int       type; };
			template <> struct make_unsigned_impl<long>        { typedef unsigned long      type; };
			template <> struct make_unsigned_impl<long long>   { typedef unsigned long long type; };
		}

		/**
		 * An alternative to @c std::make_unsigned with simplified behaviour.
		 *
		 * @note @c std::make_unsigned does not work in GCC 4.6.
		 */
		template <typename T>
			struct make_unsigned :
				detail::make_unsigned_impl<T> {};
		///@}
	}
}

#endif
