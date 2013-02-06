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

#ifndef    page_local_util_type_traits_hpp
#   define page_local_util_type_traits_hpp

#	include <cstddef> // {nullptr,ptrdiff,size}_t
#	include <iterator> // begin, end, iterator_traits
#	include <sstream> // basic_ostringstream
#	include <tuple>
#	include <type_traits>
#	include <utility> // declval

namespace page
{
	namespace util
	{
		/**
		 * @defgroup type-traits Type traits
		 * @{
		 */

////////////////////////////////////////////////////////////////////////////////

	/**
	 * @defgroup ENABLE_IF
	 *
	 * A set of macros to simplify the most common uses of @c std::enable_if for
	 * SFINAE.
	 *
	 * @sa http://lists.boost.org/Archives/boost/2011/04/180192.php
	 *
	 * @{
	 */
	/**
	 * Apply SFINAE in the declaration of a function.
	 */
#	define ENABLE_IF(PREDICATE) \
		typename std::enable_if<PREDICATE>::type * = nullptr

	/**
	 * Apply SFINAE in the definition of a function.
	 */
#	define ENABLE_IF_IMPL(PREDICATE) \
		typename std::enable_if<PREDICATE>::type *

	/**
	 * Apply SFINAE in the return value of a function (whether it is a
	 * declaration or definition).
	 */
#	define ENABLE_IF_RETURN(PREDICATE, RESULT) \
		typename std::enable_if<PREDICATE, RESULT>::type
	///@}

////////////////////////////////////////////////////////////////////////////////

	/**
	 * @defgroup DEFINE_SFINAE_TYPE_TRAIT
	 *
	 * A set of macros to make it easier to define new type-traits using SFINAE.
	 *
	 * @{
	 */
	/**
	 * Define a one-parameter type-trait using SFINAE.
	 */
#	define DEFINE_SFINAE_TYPE_TRAIT_1(NAME, TEST) \
		template <typename> class NAME; \
		namespace detail \
		{ \
			template <typename A> struct NAME##_impl \
			{ \
				template <typename T> static void test(TEST *); \
				template <typename>   static int  test(...); \
				\
				static const bool value = std::is_void<decltype(test<A>(nullptr))>::value; \
			}; \
		} \
		template <typename T> struct NAME : \
			std::integral_constant<bool, detail::NAME##_impl<T>::value> {};

	/**
	 * Define a two-parameter type-trait using SFINAE.
	 */
#	define DEFINE_SFINAE_TYPE_TRAIT_2(NAME, TEST) \
		template <typename, typename> class NAME; \
		namespace detail \
		{ \
			template <typename A, typename B> struct NAME##_impl \
			{ \
				template <typename T, typename U> static void test(TEST *); \
				template <typename,   typename>   static int  test(...); \
				\
				static const bool value = std::is_void<decltype(test<A, B>(nullptr))>::value; \
			}; \
		} \
		template <typename T, typename U> struct NAME : \
			std::integral_constant<bool, detail::NAME##_impl<T, U>::value> {};

	/**
	 * Define a three-parameter type-trait using SFINAE.
	 */
#	define DEFINE_SFINAE_TYPE_TRAIT_3(NAME, TEST) \
		template <typename, typename, typename> class NAME; \
		namespace detail \
		{ \
			template <typename A, typename B, typename C> struct NAME##_impl \
			{ \
				template <typename T, typename U, typename V> static void test(TEST *); \
				template <typename,   typename,   typename>   static int  test(...); \
				\
				static const bool value = std::is_void<decltype(test<A, B, C>(nullptr))>::value; \
			}; \
		} \
		template <typename T, typename U, typename V> struct NAME : \
			std::integral_constant<bool, detail::NAME##_impl<T, U, V>::value> {};

	/**
	 * Define a four-parameter type-trait using SFINAE.
	 */
#	define DEFINE_SFINAE_TYPE_TRAIT_4(NAME, TEST) \
		template <typename, typename, typename, typename> class NAME; \
		namespace detail \
		{ \
			template <typename A, typename B, typename C, typename D> struct NAME##_impl \
			{ \
				template <typename T, typename U, typename V, typename W> static void test(TEST *); \
				template <typename,   typename,   typename,   typename>   static int  test(...); \
				\
				static const bool value = std::is_void<decltype(test<A, B, C, D>(nullptr))>::value; \
			}; \
		} \
		template <typename T, typename U, typename V, typename W> struct NAME : \
			std::integral_constant<bool, detail::NAME##_impl<T, U, V, W>::value> {};
	///@}

////////////////////////////////////////////////////////////////////////////////

	/**
	 * @defgroup DEFINE_VARIADIC_SFINAE_TYPE_TRAIT
	 *
	 * A set of macros to make it easier to define new variadic type-traits
	 * using SFINAE.
	 *
	 * @{
	/**
	 * Define a variadic one-parameter type-trait using SFINAE.
	 */
#	define DEFINE_VARIADIC_SFINAE_TYPE_TRAIT_1(NAME, TEST) \
		template <typename, typename...> class NAME; \
		namespace detail \
		{ \
			template <typename A, typename... B> struct NAME##_impl \
			{ \
				template <typename T, typename... U> static void test(TEST *); \
				template <typename,   typename...>   static int  test(...); \
				\
				static const bool value = std::is_void<decltype(test<A, B...>(nullptr))>::value; \
			}; \
		} \
		template <typename T, typename... U> struct NAME : \
			std::integral_constant<bool, detail::NAME##_impl<T, U...>::value> {};

	/**
	 * Define a variadic two-parameter type-trait using SFINAE.
	 */
#	define DEFINE_VARIADIC_SFINAE_TYPE_TRAIT_2(NAME, TEST) \
		template <typename, typename, typename...> class NAME; \
		namespace detail \
		{ \
			template <typename A, typename B, typename... C> struct NAME##_impl \
			{ \
				template <typename T, typename U, typename... V> static void test(TEST *); \
				template <typename,   typename,   typename...>   static int  test(...); \
				\
				static const bool value = std::is_void<decltype(test<A, B, C...>(nullptr))>::value; \
			}; \
		} \
		template <typename T, typename U, typename... V> struct NAME : \
			std::integral_constant<bool, detail::NAME##_impl<T, U, V...>::value> {};
	///@}

////////////////////////////////////////////////////////////////////////////////

		/**
		 * @defgroup type-traits-container Type traits for containers
		 * @{
		 */
		/**
		 * Detect whether a type is a range, which includes any type for which
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
		 * Detect whether const is propogated to the elements of a range.
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
		 * Detect whether a type is compatible with @c std::insert_iterator.
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
		///@}

////////////////////////////////////////////////////////////////////////////////

		/**
		 * @defgroup type-traits-string Type traits for strings
		 * @{
		 */
		/**
		 * Detect whether a type is character.
		 */
		DEFINE_SFINAE_TYPE_TRAIT_1(is_character,
			typename std::enable_if<(
				std::is_same<typename std::remove_cv<T>::type, char    >::value ||
				std::is_same<typename std::remove_cv<T>::type, wchar_t >::value ||
				std::is_same<typename std::remove_cv<T>::type, char16_t>::value ||
				std::is_same<typename std::remove_cv<T>::type, char32_t>::value)>::type)

		/**
		 * Determine the character type of a string literal.
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
		 * Detect whether a type is an instantiation of @c std::basic_string.
		 */
		DEFINE_SFINAE_TYPE_TRAIT_1(is_string,
			typename std::enable_if<(std::is_same<T, std::basic_string<typename T::value_type, typename T::traits_type, typename T::allocator_type>>::value)>::type)

		/**
		 * Detect whether a type is an instantiation of @c std::basic_string,
		 * where the first template parameter matches the given parameter.
		 */
		DEFINE_SFINAE_TYPE_TRAIT_2(is_string_1,
			typename std::enable_if<(std::is_same<T, std::basic_string<U, typename T::traits_type, typename T::allocator_type>>::value)>::type)

		/**
		 * Detect whether a type is an instantiation of @c std::basic_string,
		 * where the first two template parameters match the given parameters.
		 */
		DEFINE_SFINAE_TYPE_TRAIT_3(is_string_2,
			typename std::enable_if<(std::is_same<T, std::basic_string<U, V, typename T::allocator_type>>::value)>::type)

		/**
		 * Detect whether a type is an instantiation of @c std::basic_string,
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
		 * Detect whether a type is a string literal, where the element type is
		 * a character.
		 */
		DEFINE_SFINAE_TYPE_TRAIT_1(is_string_literal,
			typename std::enable_if<(
				std::is_pointer<typename std::decay<T>::type>::value &&
				is_character<typename string_literal_char_type<T>::type>::value)>::type)

		/**
		 * Detect whether a type is a string literal, where the element type
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
		 * Detect whether a type is implicitly convertible to an instantiation
		 * of @c std::basic_string.
		 */
		DEFINE_SFINAE_TYPE_TRAIT_1(is_convertible_to_string,
			typename std::enable_if<
				is_string        <T>::value ||
				is_string_literal<T>::value>::type)

		/**
		 * Detect whether a type is implicitly convertible to an instantiation
		 * of @c std::basic_string, where the first template parameter matches
		 * the given parameter.
		 */
		DEFINE_SFINAE_TYPE_TRAIT_2(is_convertible_to_string_1,
			typename std::enable_if<(
				is_string_1        <T, U>::value ||
				is_string_literal_1<T, U>::value)>::type)
		///@}
		///@}

////////////////////////////////////////////////////////////////////////////////

		/**
		 * @defgroup type-traits-stream Type traits for streaming
		 * @{
		 */
		/**
		 * Detect whether a type can be inserted into another type, such as an
		 * output stream.
		 */
		DEFINE_SFINAE_TYPE_TRAIT_2(is_insertable,
			decltype(std::declval<U>() << std::declval<T>(), std::declval<void>()))

		/**
		 * Detect whether a type can be extracted from another type, such as an
		 * input stream.
		 */
		DEFINE_SFINAE_TYPE_TRAIT_2(is_extractable,
			decltype(std::declval<U>() >> std::declval<T>(), std::declval<void>()))
		///@}

////////////////////////////////////////////////////////////////////////////////

		/**
		 * Remove one level of indirection from a type.
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
		 * Remove the member pointer from a type, leaving the type of the member
		 * pointed to.
		 */
		template <typename T>
			struct remove_member_pointer :
				detail::remove_member_pointer_impl<T> {};
		///@}

////////////////////////////////////////////////////////////////////////////////

		/**
		 * @defgroup type-traits-integer-category Type traits for integer categorization
		 * @{
		 */
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
		 * Identify types belonging to the "integer types" category
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
		 * Identify types belonging to the "standard integer types" category
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
		///@}

////////////////////////////////////////////////////////////////////////////////

		/**
		 * @defgroup type-traits-sign-modify Type traits for sign modification
		 * @{
		 */
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
		///@}

////////////////////////////////////////////////////////////////////////////////

		/**
		 * @defgroup type-traits-cv-modify Type traits for CV-qualifier modification
		 * @{
		 */
		/**
		 * @defgroup copy_const
		 * @{
		 */
		namespace detail
		{
			template <typename To, typename From, bool = std::is_const<From>::value>
				struct copy_const_impl
			{
				typedef typename std::add_const<To>::type type;
			};
			template <typename To, typename From>
				struct copy_const_impl<To, From, false>
			{
				typedef typename std::remove_const<To>::type type;
			};
		}
		/**
		 * Copy const qualifiers from one type to another.
		 */
		template <typename To, typename From>
			struct copy_const
		{
			typedef typename detail::copy_const_impl<To, From>::type type;
		};
		///@}

		/**
		 * @defgroup copy_volatile
		 * @{
		 */
		namespace detail
		{
			template <typename To, typename From, bool = std::is_volatile<From>::value>
				struct copy_volatile_impl
			{
				typedef typename std::add_volatile<To>::type type;
			};
			template <typename To, typename From>
				struct copy_volatile_impl<To, From, false>
			{
				typedef typename std::remove_volatile<To>::type type;
			};
		}
		/**
		 * Copy volatile qualifiers from one type to another.
		 */
		template <typename To, typename From>
			struct copy_volatile
		{
			typedef typename detail::copy_volatile_impl<To, From>::type type;
		};
		///@}

		/**
		 * @defgroup copy_cv
		 * @{
		 */
		/**
		 * Copy const/volatile qualifiers from one type to another.
		 */
		template <typename To, typename From>
			struct copy_cv
		{
			typedef
				typename copy_const<
				typename copy_volatile<To, From>::type, From>::type type;
		};
		///@}
		///@}

////////////////////////////////////////////////////////////////////////////////

		/**
		 * @defgroup type-traits-iterator Type traits for iterators
		 * @{
		 */
		DEFINE_SFINAE_TYPE_TRAIT_1(has_container_type,    typename T::container_type)
		DEFINE_SFINAE_TYPE_TRAIT_1(has_iterator_category, typename T::iterator_category)
		DEFINE_SFINAE_TYPE_TRAIT_1(has_value_type,        typename T::value_type)

		/**
		 * Detect whether a type is an iterator (including a raw pointer).
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
		 * Determine the @b natural @c value_type of the output iterator.
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
		 * Detect whether a type is both an iterator and an iterator over a
		 * specific type.
		 *
		 * @deprecated
		 */
		template <typename T, typename U> struct is_iterator_over :
			detail::is_iterator_over_impl<T, U> {};
		///@}
		///@}

////////////////////////////////////////////////////////////////////////////////

		/**
		 * Generate a string containing a valid type-specifier
		 * (ISO/IEC N3242 7.1.6) for the given type.
		 *
		 * @todo This could be rewritten as a constexpr using Boost MPL for
		 *       composing the type-specifier string at compile time.
		 */
		template <typename T> struct type_string
		{
			template <typename Char = char, typename CharTraits = std::char_traits<Char>>
				static std::basic_string<Char, CharTraits> value()
			{
				std::basic_ostringstream<Char, CharTraits> ss;

				// pointers and references
				if (std::is_member_pointer<T>::value ||
					std::is_pointer       <T>::value ||
					std::is_reference     <T>::value)
				{
					// pointee/referencee type
					std::string inner(
						std::is_member_pointer<T>::value ? type_string<typename remove_member_pointer<T>::type>::template value<Char, CharTraits>() :
						std::is_pointer       <T>::value ? type_string<typename std::remove_pointer  <T>::type>::template value<Char, CharTraits>() :
						std::is_reference     <T>::value ? type_string<typename std::remove_reference<T>::type>::template value<Char, CharTraits>() : std::string());
					ss << inner;
					switch (inner.back())
					{
						case '&':
						case '*': break;
						default: ss << ' ';
					}

					// pointer/reference tokens
					ss << (
						std::is_member_pointer  <T>::value ? "<class>::*"  :
						std::is_pointer         <T>::value ? "*"           :
						std::is_lvalue_reference<T>::value ? "&"           :
						std::is_rvalue_reference<T>::value ? "&&"          : "");

					// cv qualifiers
					if ((
						std::is_member_pointer<T>::value ||
						std::is_pointer       <T>::value) && (
						std::is_const         <T>::value ||
						std::is_volatile      <T>::value))
					{
						ss << (
							std::is_const   <T>::value ? (
							std::is_volatile<T>::value ? "const volatile" : "const") :
							std::is_volatile<T>::value ? "volatile"       : "");
					}
				}
				else
				{
					// cv qualifiers
					ss << (
						std::is_const   <T>::value ? (
						std::is_volatile<T>::value ? "const volatile " : "const ") :
						std::is_volatile<T>::value ? "volatile "       : "");
					typedef typename std::remove_cv<T>::type T;

					if (std::is_fundamental<T>::value)
					{
						if (std::is_arithmetic<T>::value)
						{
							if (is_integer_type<T>::value)
							{
								if (is_standard_integer_type<T>::value ||
									is_extended_integer_type<T>::value)
								{
									// signedness
									if      (std::is_unsigned<T>::value)          ss << "unsigned ";
									else if (std::is_same<T, signed char>::value) ss << "signed ";
									typedef typename make_signed<T>::type T;

									if (is_standard_integer_type<T>::value)
									{
										ss << (
											std::is_same<T, signed char>::value ? "signed char" :
											std::is_same<T, short      >::value ? "short"       :
											std::is_same<T, int        >::value ? "int"         :
											std::is_same<T, long       >::value ? "long"        :
											std::is_same<T, long long  >::value ? "long long"   :
											"<standard-integer-type>");
									}
									else // extended integer types
									{
										ss << (
											"<extended-integer-type>");
									}
								}
								else // integer types without signedness
								{
									ss << (
										std::is_same<T, bool    >::value ? "bool"     :
										std::is_same<T, char    >::value ? "char"     :
										std::is_same<T, char16_t>::value ? "char16_t" :
										std::is_same<T, char32_t>::value ? "char32_t" :
										std::is_same<T, wchar_t >::value ? "wchar_t"  :
										"<integer-type>");
								}
							}
							else if (std::is_floating_point<T>::value)
							{
								ss << (
									std::is_same<T, float      >::value ? "float"       :
									std::is_same<T, double     >::value ? "double"      :
									std::is_same<T, long double>::value ? "long double" :
									"<float-type>");
							}
							else ss << "<arithmetic-type>";
						}
						else
						{
							ss << (
								std::is_void<T>::value ? "void" :
									"<fundamental-type>");
						}
					}
					else
					{
						ss << (
							std::is_same<T, std::nullptr_t  >::value ? "std::nullptr_t"   :
							std::is_same<T, std::ptrdiff_t  >::value ? "std::ptrdiff_t"   :
							std::is_same<T, std::size_t     >::value ? "std::size_t"      :
							"<type>");
					}
				}
				return ss.str();
			}
		};

////////////////////////////////////////////////////////////////////////////////

		///@}
	}
}

#endif
