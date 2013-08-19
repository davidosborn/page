#ifndef    page_local_util_type_traits_string_hpp
#   define page_local_util_type_traits_string_hpp

#	include <string>
#	include <type_traits>

#	include "sfinae.hpp"

namespace page
{
	namespace util
	{
		/**
		 * Detects whether a type is character.
		 */
		DEFINE_SFINAE_TYPE_TRAIT_1(is_character,
			typename std::enable_if<(
				std::is_same<typename std::remove_cv<T>::type, char    >::value ||
				std::is_same<typename std::remove_cv<T>::type, wchar_t >::value ||
				std::is_same<typename std::remove_cv<T>::type, char16_t>::value ||
				std::is_same<typename std::remove_cv<T>::type, char32_t>::value)>::type)

		/**
		 * Determines the character type of a string literal.
		 */
		template <typename T>
			struct string_literal_char_type
		{
			typedef
				typename std::remove_cv<
					typename std::remove_pointer<
						typename std::decay<T>::type
						>::type
					>::type type;
		};

		/**
		 * @defgroup is_string
		 * @{
		 */
		/**
		 * Detects whether a type is an instantiation of @c std::basic_string.
		 */
		DEFINE_SFINAE_TYPE_TRAIT_1(is_string,
			typename std::enable_if<(std::is_same<T, std::basic_string<typename T::value_type, typename T::traits_type, typename T::allocator_type>>::value)>::type)

		/**
		 * Detects whether a type is an instantiation of @c std::basic_string,
		 * where the first template parameter matches the given parameter.
		 */
		DEFINE_SFINAE_TYPE_TRAIT_2(is_string_1,
			typename std::enable_if<(std::is_same<T, std::basic_string<U, typename T::traits_type, typename T::allocator_type>>::value)>::type)

		/**
		 * Detects whether a type is an instantiation of @c std::basic_string,
		 * where the first two template parameters match the given parameters.
		 */
		DEFINE_SFINAE_TYPE_TRAIT_3(is_string_2,
			typename std::enable_if<(std::is_same<T, std::basic_string<U, V, typename T::allocator_type>>::value)>::type)

		/**
		 * Detects whether a type is an instantiation of @c std::basic_string,
		 * where all three template parameters match the given parameters.
		 */
		DEFINE_SFINAE_TYPE_TRAIT_4(is_string_3,
			typename std::enable_if<(std::is_same<T, std::basic_string<U, V, W>>::value)>::type)
		///@}

		/**
		 * @defgroup is_string_literal
		 * @{
		 */
		/**
		 * Detects whether a type is a string literal, where the element type is
		 * a character.
		 */
		DEFINE_SFINAE_TYPE_TRAIT_1(is_string_literal,
			typename std::enable_if<(
				std::is_pointer<typename std::decay<T>::type>::value &&
				is_character<typename string_literal_char_type<T>::type>::value)>::type)

		/**
		 * Detects whether a type is a string literal, where the element type
		 * matches the second template parameter.
		 */
		DEFINE_SFINAE_TYPE_TRAIT_2(is_string_literal_1,
			typename std::enable_if<(
				std::is_pointer<typename std::decay<T>::type>::value &&
				std::is_same<typename string_literal_char_type<T>::type, U>::value)>::type)
		///@}

		/**
		 * @defgroup is_implicitly_convertible_to_string
		 * @{
		 */
		/**
		 * Detects whether a type is implicitly convertible to an instantiation
		 * of @c std::basic_string.
		 */
		DEFINE_SFINAE_TYPE_TRAIT_1(is_convertible_to_string,
			typename std::enable_if<
				is_string        <T>::value ||
				is_string_literal<T>::value>::type)

		/**
		 * Detects whether a type is implicitly convertible to an instantiation
		 * of @c std::basic_string, where the first template parameter matches
		 * the given parameter.
		 */
		DEFINE_SFINAE_TYPE_TRAIT_2(is_convertible_to_string_1,
			typename std::enable_if<(
				is_string_1        <T, U>::value ||
				is_string_literal_1<T, U>::value)>::type)
		///@}
	}
}

#endif
