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

#include "serialize/deserialize_string.hpp" // Deserialize
#include "serialize/serialize_string.hpp" // Serialize
#include "type_traits/sfinae.hpp" // ENABLE_IF
#include "type_traits/string.hpp" // is_string

namespace page
{
	namespace util
	{
		namespace detail
		{
			/*template <typename To, typename From>
				struct LexicalCastImpl
			{
				static To Apply(const From &from)
				{
					return static_cast<To>(from);
				}
			};
			template <
				typename ToChar, typename ToCharTraits, typename ToAllocator,
				typename From>
				struct LexicalCastImpl<
					std::basic_string<ToChar, ToCharTraits, ToAllocator>,
					From>
			{
				typedef std::basic_string<ToChar, ToCharTraits, ToAllocator> To;
				
				static To Apply(const From &from)
				{
					return Serialize<ToChar, ToCharTraits, ToAllocator>(from);
				}
			};
			template <
				typename To,
				typename FromChar, typename FromCharTraits, typename FromAllocator>
				struct LexicalCastImpl<
					To,
					std::basic_string<FromChar, FromCharTraits, FromAllocator>>
			{
				typedef std::basic_string<FromChar, FromCharTraits, FromAllocator> From;

				static To Apply(const From &from)
				{
					To to;
					Deserialize(from, to);
					return to;
				}
			};
			template <
				typename   ToChar, typename   ToCharTraits, typename   ToAllocator,
				typename FromChar, typename FromCharTraits, typename FromAllocator>
				struct LexicalCastImpl<
					std::basic_string<  ToChar,   ToCharTraits,   ToAllocator>,
					std::basic_string<FromChar, FromCharTraits, FromAllocator>>
			{
				typedef std::basic_string<  ToChar,   ToCharTraits,   ToAllocator> To;
				typedef std::basic_string<FromChar, FromCharTraits, FromAllocator> From;
			
				static To Apply(const From &from)
				{
					return Serialize<ToChar, ToCharTraits, ToAllocator>(from);
				}
			};*/
			
			template <typename To, typename From>
				To LexicalCastImpl(const From &from,
					ENABLE_IF(
						!is_string<To>::value &&
						!is_string<From>::value))
			{
				return static_cast<To>(from);
			}
			template <typename To, typename From>
				To LexicalCastImpl(const From &from,
					ENABLE_IF(is_string<To>::value))
			{
				return Serialize<
					typename To::    value_type,
					typename To::   traits_type,
					typename To::allocator_type>(from);
			}
			template <typename To, typename From>
				To LexicalCastImpl(const From &from,
					ENABLE_IF(
						!is_string<To>::value &&
						is_string<From>::value))
			{
				To to;
				Deserialize(from, to);
				return to;
			}
		}

		// lexical cast
		template <typename To, typename From> To lexical_cast(const From &from)
		{
			return detail::LexicalCastImpl<To>(from);
		}
	}
}
