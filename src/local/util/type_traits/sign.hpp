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
