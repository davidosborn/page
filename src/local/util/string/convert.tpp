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

#include <locale>

#include "../../err/Exception.hpp"

namespace page
{
	namespace util
	{
		namespace detail
		{
			template <typename From, typename To>
				class ConvertImpl;

			template <
				typename FromCharTraits, typename FromAllocator,
				typename   ToCharTraits, typename   ToAllocator>
				struct ConvertImpl<
					std::basic_string<char, FromCharTraits, FromAllocator>,
					std::basic_string<char,   ToCharTraits,   ToAllocator>>
			{
				typedef std::basic_string<char, FromCharTraits, FromAllocator> From;
				typedef std::basic_string<char,   ToCharTraits,   ToAllocator>   To;

				static To Apply(const From &from)
				{
					return from; // identity
				}
			};

			template <
				typename FromCharTraits, typename FromAllocator,
				typename   ToCharTraits, typename   ToAllocator>
				struct ConvertImpl<
					std::basic_string<char,  FromCharTraits, FromAllocator>,
					std::basic_string<wchar_t, ToCharTraits,   ToAllocator>>
			{
				typedef std::basic_string<char,  FromCharTraits, FromAllocator> From;
				typedef std::basic_string<wchar_t, ToCharTraits,   ToAllocator>   To;

				static To Apply(const From &from)
				{
					//return std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().from_bytes(from);
				}
			};

			template <
				typename FromCharTraits, typename FromAllocator,
				typename   ToCharTraits, typename   ToAllocator>
				struct ConvertImpl<
					std::basic_string<char,   FromCharTraits, FromAllocator>,
					std::basic_string<char16_t, ToCharTraits,   ToAllocator>>
			{
				typedef std::basic_string<char,   FromCharTraits, FromAllocator> From;
				typedef std::basic_string<char16_t, ToCharTraits,   ToAllocator>   To;

				static To Apply(const From &from)
				{
					//return std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>().from_bytes(from);
				}
			};

			template <
				typename FromCharTraits, typename FromAllocator,
				typename   ToCharTraits, typename   ToAllocator>
				struct ConvertImpl<
					std::basic_string<char,   FromCharTraits, FromAllocator>,
					std::basic_string<char32_t, ToCharTraits,   ToAllocator>>
			{
				typedef std::basic_string<char,   FromCharTraits, FromAllocator> From;
				typedef std::basic_string<char32_t, ToCharTraits,   ToAllocator>   To;

				static To Apply(const From &from)
				{
					//return std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t>().from_bytes(from);
				}
			};

			template <
				typename FromCharTraits, typename FromAllocator,
				typename   ToCharTraits, typename   ToAllocator>
				struct ConvertImpl<
					std::basic_string<wchar_t, FromCharTraits, FromAllocator>,
					std::basic_string<char,      ToCharTraits,   ToAllocator>>
			{
				typedef std::basic_string<wchar_t, FromCharTraits, FromAllocator> From;
				typedef std::basic_string<char,      ToCharTraits,   ToAllocator>   To;

				static To Apply(const From &from)
				{
					//return std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(from);
				}
			};

			template <
				typename FromCharTraits, typename FromAllocator,
				typename   ToCharTraits, typename   ToAllocator>
				struct ConvertImpl<
					std::basic_string<wchar_t, FromCharTraits, FromAllocator>,
					std::basic_string<wchar_t,   ToCharTraits,   ToAllocator>>
			{
				typedef std::basic_string<wchar_t, FromCharTraits, FromAllocator> From;
				typedef std::basic_string<wchar_t,   ToCharTraits,   ToAllocator>   To;

				static To Apply(const From &from)
				{
					return from; // identity
				}
			};

			template <
				typename FromCharTraits, typename FromAllocator,
				typename   ToCharTraits, typename   ToAllocator>
				struct ConvertImpl<
					std::basic_string<wchar_t, FromCharTraits, FromAllocator>,
					std::basic_string<char16_t,  ToCharTraits,   ToAllocator>>
			{
				typedef std::basic_string<wchar_t, FromCharTraits, FromAllocator> From;
				typedef std::basic_string<char16_t,  ToCharTraits,   ToAllocator>   To;

				static To Apply(const From &from)
				{
					//return std::wstring_convert<std::codecvt_utf16<wchar_t>, wchar_t>().to_bytes(from);
				}
			};

			template <
				typename FromCharTraits, typename FromAllocator,
				typename   ToCharTraits, typename   ToAllocator>
				struct ConvertImpl<
					std::basic_string<wchar_t, FromCharTraits, FromAllocator>,
					std::basic_string<char32_t,  ToCharTraits,   ToAllocator>>
			{
				typedef std::basic_string<wchar_t, FromCharTraits, FromAllocator> From;
				typedef std::basic_string<char32_t,  ToCharTraits,   ToAllocator>   To;

				static To Apply(const From &from)
				{
					// UCS-2 -> UTF-8 -> UTF-32
					/*return std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t>().from_bytes(
						std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(from));*/
				}
			};

			template <
				typename FromCharTraits, typename FromAllocator,
				typename   ToCharTraits, typename   ToAllocator>
				struct ConvertImpl<
					std::basic_string<char16_t, FromCharTraits, FromAllocator>,
					std::basic_string<char,       ToCharTraits,   ToAllocator>>
			{
				typedef std::basic_string<char16_t, FromCharTraits, FromAllocator> From;
				typedef std::basic_string<char,       ToCharTraits,   ToAllocator>   To;

				static To Apply(const From &from)
				{
					//return std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>().to_bytes(from);
				}
			};

			template <
				typename FromCharTraits, typename FromAllocator,
				typename   ToCharTraits, typename   ToAllocator>
				struct ConvertImpl<
					std::basic_string<char16_t, FromCharTraits, FromAllocator>,
					std::basic_string<wchar_t,    ToCharTraits,   ToAllocator>>
			{
				typedef std::basic_string<char16_t, FromCharTraits, FromAllocator> From;
				typedef std::basic_string<wchar_t,    ToCharTraits,   ToAllocator>   To;

				static To Apply(const From &from)
				{
					//return std::wstring_convert<std::codecvt_utf16<wchar_t>, wchar_t>().from_bytes(from);
				}
			};

			template <
				typename FromCharTraits, typename FromAllocator,
				typename   ToCharTraits, typename   ToAllocator>
				struct ConvertImpl<
					std::basic_string<char16_t, FromCharTraits, FromAllocator>,
					std::basic_string<char16_t,   ToCharTraits,   ToAllocator>>
			{
				typedef std::basic_string<char16_t, FromCharTraits, FromAllocator> From;
				typedef std::basic_string<char16_t,   ToCharTraits,   ToAllocator>   To;

				static To Apply(const From &from)
				{
					return from; // identity
				}
			};

			template <
				typename FromCharTraits, typename FromAllocator,
				typename   ToCharTraits, typename   ToAllocator>
				struct ConvertImpl<
					std::basic_string<char16_t, FromCharTraits, FromAllocator>,
					std::basic_string<char32_t,   ToCharTraits,   ToAllocator>>
			{
				typedef std::basic_string<char16_t, FromCharTraits, FromAllocator> From;
				typedef std::basic_string<char32_t,   ToCharTraits,   ToAllocator>   To;

				static To Apply(const From &from)
				{
					// UTF-16 -> UTF-8 -> UTF-32
					/*return std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t>().from_bytes(
						std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>().to_bytes(from));*/
				}
			};

			template <
				typename FromCharTraits, typename FromAllocator,
				typename   ToCharTraits, typename   ToAllocator>
				struct ConvertImpl<
					std::basic_string<char32_t, FromCharTraits, FromAllocator>,
					std::basic_string<char,       ToCharTraits,   ToAllocator>>
			{
				typedef std::basic_string<char32_t, FromCharTraits, FromAllocator> From;
				typedef std::basic_string<char,       ToCharTraits,   ToAllocator>   To;

				static To Apply(const From &from)
				{
					//return std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t>().to_bytes(from);
				}
			};

			template <
				typename FromCharTraits, typename FromAllocator,
				typename   ToCharTraits, typename   ToAllocator>
				struct ConvertImpl<
					std::basic_string<char32_t, FromCharTraits, FromAllocator>,
					std::basic_string<wchar_t,    ToCharTraits,   ToAllocator>>
			{
				typedef std::basic_string<char32_t, FromCharTraits, FromAllocator> From;
				typedef std::basic_string<wchar_t,    ToCharTraits,   ToAllocator>   To;

				static To Apply(const From &from)
				{
					// UTF-32 -> UTF-8 -> UCS-2
					/*return std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().from_bytes(
						std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t>().to_bytes(from));*/
				}
			};

			template <
				typename FromCharTraits, typename FromAllocator,
				typename   ToCharTraits, typename   ToAllocator>
				struct ConvertImpl<
					std::basic_string<char32_t, FromCharTraits, FromAllocator>,
					std::basic_string<char16_t,   ToCharTraits,   ToAllocator>>
			{
				typedef std::basic_string<char32_t, FromCharTraits, FromAllocator> From;
				typedef std::basic_string<char16_t,   ToCharTraits,   ToAllocator>   To;

				static To Apply(const From &from)
				{
					// UTF-32 -> UTF-8 -> UTF-16
					/*return std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>().from_bytes(
						std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t>().to_bytes(from));*/
				}
			};

			template <
				typename FromCharTraits, typename FromAllocator,
				typename   ToCharTraits, typename   ToAllocator>
				struct ConvertImpl<
					std::basic_string<char32_t, FromCharTraits, FromAllocator>,
					std::basic_string<char32_t,   ToCharTraits,   ToAllocator>>
			{
				typedef std::basic_string<char32_t, FromCharTraits, FromAllocator> From;
				typedef std::basic_string<char32_t,   ToCharTraits,   ToAllocator>   To;

				static To Apply(const From &from)
				{
					return from; // identity
				}
			};
		}

		template <
			typename   ToChar, typename   ToCharTraits, typename   ToAllocator,
			typename FromChar, typename FromCharTraits, typename FromAllocator>
				std::basic_string<ToChar, ToCharTraits, ToAllocator>
				Convert(const std::basic_string<FromChar, FromCharTraits, FromAllocator> &from)
		{
			try
			{
				return detail::ConvertImpl<
					std::basic_string<FromChar, FromCharTraits, FromAllocator>,
					std::basic_string<  ToChar,   ToCharTraits,   ToAllocator>>::
					Apply(from);
			}
			catch (const std::range_error &)
			{
				THROW((err::Exception<err::UtilModuleTag, err::ConversionTag>()))
			}
		}

		template <
			typename   ToChar, typename   ToCharTraits, typename   ToAllocator,
			typename FromChar, typename FromCharTraits, typename FromAllocator>
				std::basic_string<ToChar, ToCharTraits, ToAllocator>
				Convert(const FromChar *from)
		{
			return Convert<ToChar, ToCharTraits, ToAllocator>(
				std::basic_string<FromChar, FromCharTraits, FromAllocator>(from));
		}
	}
}
