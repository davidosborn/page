/**
 * @section license
 *
 * Copyright (c) 2006-2013 David Osborn
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

#ifndef    page_local_util_type_traits_integer_hpp
#   define page_local_util_type_traits_integer_hpp

#	include <type_traits>

namespace page
{
	namespace util
	{
		/**
		 * @defgroup is_integer_type
		 * @{
		 */
		namespace detail
		{
			template <typename T>
				struct is_integer_type_impl :
					std::is_integral<T> {};
		}

		/**
		 * Identifies types belonging to the "integer types" category
		 * (ISO/IEC N3242 3.9.1.7).
		 */
		template <typename T>
			struct is_integer_type :
				detail::is_integer_type_impl<T> {};
		///@}

		/**
		 * @defgroup is_standard_integer_type
		 * @{
		 */
		namespace detail
		{
			template <typename T>
				struct is_standard_integer_type_impl_2 :
					std::integral_constant<bool,
						std::is_same<T,   signed char     >::value ||
						std::is_same<T, unsigned char     >::value ||
						std::is_same<T,          short    >::value ||
						std::is_same<T, unsigned short    >::value ||
						std::is_same<T,          int      >::value ||
						std::is_same<T, unsigned int      >::value ||
						std::is_same<T,          long     >::value ||
						std::is_same<T, unsigned long     >::value ||
						std::is_same<T,          long long>::value ||
						std::is_same<T, unsigned long long>::value> {};
			template <typename T>
				struct is_standard_integer_type_impl :
					is_standard_integer_type_impl_2<
						typename std::remove_cv<T>::type> {};
		}

		/**
		 * Identifies types belonging to the "standard integer types" category
		 * (ISO/IEC N3242 3.9.1.3).
		 */
		template <typename T>
			struct is_standard_integer_type :
				detail::is_standard_integer_type_impl<T> {};
		///@}

		/**
		 * @defgroup is_extended_integer_type
		 * @{
		 */
		namespace detail
		{
			template <typename T>
				struct is_extended_integer_type_impl_2 :
					std::integral_constant<bool,
						 util::is_integer_type         <T>::value &&
						!util::is_standard_integer_type<T>::value &&
						!(
							std::is_same<T, bool    >::value ||
							std::is_same<T, char    >::value ||
							std::is_same<T, char16_t>::value ||
							std::is_same<T, char32_t>::value ||
							std::is_same<T, wchar_t >::value)> {};
			template <typename T>
				struct is_extended_integer_type_impl :
					is_extended_integer_type_impl_2<
						typename std::remove_cv<T>::type> {};
		}

		/**
		 * Identifies types belonging to the "extended integer types" category
		 * (ISO/IEC N3242 3.9.1.3).
		 */
		template <typename T>
			struct is_extended_integer_type :
				detail::is_extended_integer_type_impl<T> {};
		///@}
	}
}

#endif
