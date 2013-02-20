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

#include "../../err/Exception.hpp"

namespace page
{
	namespace util
	{
		// Convert
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
				static std::basic_string<char, ToCharTraits, ToAllocator> Apply(
					const std::basic_string<char, FromCharTraits, FromAllocator> &from,
					typename ConvertFlags::Type fromFlags,
					typename ConvertFlags::Type   toFlags)
				{
					if ((fromFlags & ConvertFlags::variableWidth) ==
						(  toFlags & ConvertFlags::variableWidth))
					{
						return from;
					}
					else
					{
						// FIXME: implement
						THROW((err::Exception<err::UtilModuleTag, err::ConversionTag>()))
					}
				}
			};
			template <
				typename FromCharTraits, typename FromAllocator,
				typename   ToCharTraits, typename   ToAllocator>
				struct ConvertImpl<
					std::basic_string<char,  FromCharTraits, FromAllocator>,
					std::basic_string<wchar_t, ToCharTraits,   ToAllocator>>
			{
				static std::basic_string<wchar_t, ToCharTraits, ToAllocator> Apply(
					const std::basic_string<char, FromCharTraits, FromAllocator> &from,
					typename ConvertFlags::Type fromFlags,
					typename ConvertFlags::Type   toFlags)
				{
					// FIXME: implement
					// http://en.cppreference.com/w/cpp/locale/wstring_convert
					// http://stackoverflow.com/questions/7232710/convert-between-string-u16string-u32string
					THROW((err::Exception<err::UtilModuleTag, err::ConversionTag>()))
				}
			};
			template <
				typename FromCharTraits, typename FromAllocator,
				typename   ToCharTraits, typename   ToAllocator>
				struct ConvertImpl<
					std::basic_string<char,   FromCharTraits, FromAllocator>,
					std::basic_string<char16_t, ToCharTraits,   ToAllocator>>
			{
				static std::basic_string<char16_t, ToCharTraits, ToAllocator> Apply(
					const std::basic_string<char, FromCharTraits, FromAllocator> &from,
					typename ConvertFlags::Type fromFlags,
					typename ConvertFlags::Type   toFlags)
				{
					// FIXME: implement
					THROW((err::Exception<err::UtilModuleTag, err::ConversionTag>()))
				}
			};
			template <
				typename FromCharTraits, typename FromAllocator,
				typename   ToCharTraits, typename   ToAllocator>
				struct ConvertImpl<
					std::basic_string<char,   FromCharTraits, FromAllocator>,
					std::basic_string<char32_t, ToCharTraits,   ToAllocator>>
			{
				static std::basic_string<char32_t, ToCharTraits, ToAllocator> Apply(
					const std::basic_string<char, FromCharTraits, FromAllocator> &from,
					typename ConvertFlags::Type fromFlags,
					typename ConvertFlags::Type   toFlags)
				{
					// FIXME: implement
					THROW((err::Exception<err::UtilModuleTag, err::ConversionTag>()))
				}
			};
			template <
				typename FromCharTraits, typename FromAllocator,
				typename   ToCharTraits, typename   ToAllocator>
				struct ConvertImpl<
					std::basic_string<wchar_t, FromCharTraits, FromAllocator>,
					std::basic_string<char,      ToCharTraits,   ToAllocator>>
			{
				static std::basic_string<char, ToCharTraits, ToAllocator> Apply(
					const std::basic_string<wchar_t, FromCharTraits, FromAllocator> &from,
					typename ConvertFlags::Type fromFlags,
					typename ConvertFlags::Type   toFlags)
				{
					// FIXME: implement
					THROW((err::Exception<err::UtilModuleTag, err::ConversionTag>()))
				}
			};
			template <
				typename FromCharTraits, typename FromAllocator,
				typename   ToCharTraits, typename   ToAllocator>
				struct ConvertImpl<
					std::basic_string<wchar_t, FromCharTraits, FromAllocator>,
					std::basic_string<wchar_t,   ToCharTraits,   ToAllocator>>
			{
				static std::basic_string<wchar_t, ToCharTraits, ToAllocator> Apply(
					const std::basic_string<wchar_t, FromCharTraits, FromAllocator> &from,
					typename ConvertFlags::Type fromFlags,
					typename ConvertFlags::Type   toFlags)
				{
					// FIXME: implement
					THROW((err::Exception<err::UtilModuleTag, err::ConversionTag>()))
				}
			};
			template <
				typename FromCharTraits, typename FromAllocator,
				typename   ToCharTraits, typename   ToAllocator>
				struct ConvertImpl<
					std::basic_string<wchar_t, FromCharTraits, FromAllocator>,
					std::basic_string<char16_t,  ToCharTraits,   ToAllocator>>
			{
				static std::basic_string<char16_t, ToCharTraits, ToAllocator> Apply(
					const std::basic_string<wchar_t, FromCharTraits, FromAllocator> &from,
					typename ConvertFlags::Type fromFlags,
					typename ConvertFlags::Type   toFlags)
				{
					// FIXME: implement
					THROW((err::Exception<err::UtilModuleTag, err::ConversionTag>()))
				}
			};
			template <
				typename FromCharTraits, typename FromAllocator,
				typename   ToCharTraits, typename   ToAllocator>
				struct ConvertImpl<
					std::basic_string<wchar_t, FromCharTraits, FromAllocator>,
					std::basic_string<char32_t,  ToCharTraits,   ToAllocator>>
			{
				static std::basic_string<char32_t, ToCharTraits, ToAllocator> Apply(
					const std::basic_string<wchar_t, FromCharTraits, FromAllocator> &from,
					typename ConvertFlags::Type fromFlags,
					typename ConvertFlags::Type   toFlags)
				{
					// FIXME: implement
					THROW((err::Exception<err::UtilModuleTag, err::ConversionTag>()))
				}
			};
			template <
				typename FromCharTraits, typename FromAllocator,
				typename   ToCharTraits, typename   ToAllocator>
				struct ConvertImpl<
					std::basic_string<char16_t, FromCharTraits, FromAllocator>,
					std::basic_string<char,       ToCharTraits,   ToAllocator>>
			{
				static std::basic_string<char, ToCharTraits, ToAllocator> Apply(
					const std::basic_string<char16_t, FromCharTraits, FromAllocator> &from,
					typename ConvertFlags::Type fromFlags,
					typename ConvertFlags::Type   toFlags)
				{
					// FIXME: implement
					THROW((err::Exception<err::UtilModuleTag, err::ConversionTag>()))
				}
			};
			template <
				typename FromCharTraits, typename FromAllocator,
				typename   ToCharTraits, typename   ToAllocator>
				struct ConvertImpl<
					std::basic_string<char16_t, FromCharTraits, FromAllocator>,
					std::basic_string<wchar_t,    ToCharTraits,   ToAllocator>>
			{
				static std::basic_string<wchar_t, ToCharTraits, ToAllocator> Apply(
					const std::basic_string<char16_t, FromCharTraits, FromAllocator> &from,
					typename ConvertFlags::Type fromFlags,
					typename ConvertFlags::Type   toFlags)
				{
					// FIXME: implement
					THROW((err::Exception<err::UtilModuleTag, err::ConversionTag>()))
				}
			};
			template <
				typename FromCharTraits, typename FromAllocator,
				typename   ToCharTraits, typename   ToAllocator>
				struct ConvertImpl<
					std::basic_string<char16_t, FromCharTraits, FromAllocator>,
					std::basic_string<char16_t,   ToCharTraits,   ToAllocator>>
			{
				static std::basic_string<char16_t, ToCharTraits, ToAllocator> Apply(
					const std::basic_string<char16_t, FromCharTraits, FromAllocator> &from,
					typename ConvertFlags::Type fromFlags,
					typename ConvertFlags::Type   toFlags)
				{
					// FIXME: implement
					THROW((err::Exception<err::UtilModuleTag, err::ConversionTag>()))
				}
			};
			template <
				typename FromCharTraits, typename FromAllocator,
				typename   ToCharTraits, typename   ToAllocator>
				struct ConvertImpl<
					std::basic_string<char16_t, FromCharTraits, FromAllocator>,
					std::basic_string<char32_t,   ToCharTraits,   ToAllocator>>
			{
				static std::basic_string<char32_t, ToCharTraits, ToAllocator> Apply(
					const std::basic_string<char16_t, FromCharTraits, FromAllocator> &from,
					typename ConvertFlags::Type fromFlags,
					typename ConvertFlags::Type   toFlags)
				{
					// FIXME: implement
					THROW((err::Exception<err::UtilModuleTag, err::ConversionTag>()))
				}
			};
			template <
				typename FromCharTraits, typename FromAllocator,
				typename   ToCharTraits, typename   ToAllocator>
				struct ConvertImpl<
					std::basic_string<char32_t, FromCharTraits, FromAllocator>,
					std::basic_string<char,       ToCharTraits,   ToAllocator>>
			{
				static std::basic_string<char, ToCharTraits, ToAllocator> Apply(
					const std::basic_string<char32_t, FromCharTraits, FromAllocator> &from,
					typename ConvertFlags::Type fromFlags,
					typename ConvertFlags::Type   toFlags)
				{
					// FIXME: implement
					THROW((err::Exception<err::UtilModuleTag, err::ConversionTag>()))
				}
			};
			template <
				typename FromCharTraits, typename FromAllocator,
				typename   ToCharTraits, typename   ToAllocator>
				struct ConvertImpl<
					std::basic_string<char32_t, FromCharTraits, FromAllocator>,
					std::basic_string<wchar_t,    ToCharTraits,   ToAllocator>>
			{
				static std::basic_string<wchar_t, ToCharTraits, ToAllocator> Apply(
					const std::basic_string<char32_t, FromCharTraits, FromAllocator> &from,
					typename ConvertFlags::Type fromFlags,
					typename ConvertFlags::Type   toFlags)
				{
					// FIXME: implement
					THROW((err::Exception<err::UtilModuleTag, err::ConversionTag>()))
				}
			};
			template <
				typename FromCharTraits, typename FromAllocator,
				typename   ToCharTraits, typename   ToAllocator>
				struct ConvertImpl<
					std::basic_string<char32_t, FromCharTraits, FromAllocator>,
					std::basic_string<char16_t,   ToCharTraits,   ToAllocator>>
			{
				static std::basic_string<char16_t, ToCharTraits, ToAllocator> Apply(
					const std::basic_string<char32_t, FromCharTraits, FromAllocator> &from,
					typename ConvertFlags::Type fromFlags,
					typename ConvertFlags::Type   toFlags)
				{
					// FIXME: implement
					THROW((err::Exception<err::UtilModuleTag, err::ConversionTag>()))
				}
			};
			template <
				typename FromCharTraits, typename FromAllocator,
				typename   ToCharTraits, typename   ToAllocator>
				struct ConvertImpl<
					std::basic_string<char32_t, FromCharTraits, FromAllocator>,
					std::basic_string<char32_t,   ToCharTraits,   ToAllocator>>
			{
				static std::basic_string<char32_t, ToCharTraits, ToAllocator> Apply(
					const std::basic_string<char32_t, FromCharTraits, FromAllocator> &from,
					typename ConvertFlags::Type fromFlags,
					typename ConvertFlags::Type   toFlags)
				{
					// FIXME: implement
					THROW((err::Exception<err::UtilModuleTag, err::ConversionTag>()))
				}
			};
		}
		template <
			typename   ToChar, typename   ToCharTraits, typename   ToAllocator,
			typename FromChar, typename FromCharTraits, typename FromAllocator>
			std::basic_string<ToChar, ToCharTraits, ToAllocator> Convert(
				const std::basic_string<FromChar, FromCharTraits, FromAllocator> &from,
				typename ConvertFlags::Type flags = ConvertFlags::none)
		{
			return Convert<ToChar, ToCharTraits, ToAllocator>(from, flags, flags);
		}
		template <
			typename   ToChar, typename   ToCharTraits, typename   ToAllocator,
			typename FromChar, typename FromCharTraits, typename FromAllocator>
			std::basic_string<ToChar, ToCharTraits, ToAllocator> Convert(
				const std::basic_string<FromChar, FromCharTraits, FromAllocator> &from,
				typename ConvertFlags::Type fromFlags = ConvertFlags::none,
				typename ConvertFlags::Type   toFlags = ConvertFlags::none)
		{
			return detail::ConvertImpl<
				std::basic_string<FromChar, FromCharTraits, FromAllocator>,
				std::basic_string<  ToChar,   ToCharTraits,   ToAllocator>>::
				Apply(from, fromFlags, toFlags);
		}
	}
}
