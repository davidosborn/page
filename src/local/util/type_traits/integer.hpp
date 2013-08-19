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
