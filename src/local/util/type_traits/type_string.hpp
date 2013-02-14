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

#ifndef    page_local_util_type_traits_type_string_hpp
#   define page_local_util_type_traits_type_string_hpp

#	include <cstddef> // {nullptr,ptrdiff,size}_t
#	include <sstream> // basic_ostringstream, basic_string
#	include <type_traits>

#	include "integer.hpp"

namespace page
{
	namespace util
	{
		/**
		 * Generates a string containing a valid type-specifier
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
	}
}

#endif
