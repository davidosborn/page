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

#ifndef    page_local_util_locale_CharEncoding_hpp
#   define page_local_util_locale_CharEncoding_hpp

#	include "../type_traits/string.hpp" // is_character

namespace page
{
	namespace util
	{
		/**
		 * @note When the byte-order is not specified as part of the encoding,
		 *       it should be determined by checking for the existence of a BOM,
		 *       otherwise defaulting to the native byte-order of the platform.
		 */
		enum class CharEncoding
		{
			/**
			 * Specifies the default encoding for the data type.
			 *
			 * The default encoding is
			 * @c locale_char    for @c char,
			 * @c locale_wchar_t for @c wchar_t,
			 * @c utf16          for @c char16_t, and
			 * @c utf32          for @c char32_t.
			 *
			 * @note This encoding is partially specified -- it depends on the
			 *       data type for resolution.
			 */
			defaultForType,

			/**
			 * Specifies the ASCII-7 encoding.
			 */
			ascii,

			/**
			 * Specifies the locale-defined encoding for @c char.
			 */
			locale_char,

			/**
			 * Specifies the locale-defined encoding for @c wchar_t.
			 */
			locale_wchar_t,

			/**
			 * Specifies the big-endian UCS-2 encoding.
			 * @copydetails ucs2
			 */
			ucs2be,

			/**
			 * Specifies the little-endian UCS-2 encoding.
			 * @copydetails ucs2
			 */
			ucs2le,

			/**
			 * Specifies the UCS-2 encoding.
			 * @note UCS-2 is equivalent to UTF-16 without surrogates.
			 */
			ucs2 =
#ifdef WORDS_BIGENDIAN
				ucs2be,
#else
				ucs2le,
#endif

			/**
			 * Specifies a unicode encoding.
			 *
			 * @note This encoding is partially specified -- it depends on the
			 *       data type for resolution.
			 */
			utf,

			/**
			 * Specifies the UTF-8 encoding.
			 */
			utf8,

			/**
			 * Specifies the big-endian UTF-16 encoding.
			 * @copydetails utf16
			 */
			utf16be,

			/**
			 * Specifies the little-endian UTF-16 encoding.
			 * @copydetails utf16
			 */
			utf16le,

			/**
			 * Specifies the UTF-16 encoding.
			 */
			utf16 =
#ifdef WORDS_BIGENDIAN
				utf16be,
#else
				utf16le,
#endif

			/**
			 * Specifies the big-endian UTF-32 encoding.
			 * @copydetails utf32
			 */
			utf32be,

			/**
			 * Specifies the little-endian UTF-32 encoding.
			 * @copydetails utf32
			 */
			utf32le,

			/**
			 * Specifies the UTF-32 encoding.
			 */
			utf32 =
#ifdef WORDS_BIGENDIAN
				utf32be
#else
				utf32le
#endif
		};

		////////////////////////////////////////////////////////////////////////

		/**
		 * Specifies the natural data-type for a @c CharEncoding.
		 */
		template <CharEncoding encoding>
			struct GetCharEncodingType
		{
			static_assert(encoding != encoding,
				"partially-specified encoding has no type");
		};

		template <>
			struct GetCharEncodingType<CharEncoding::ascii>
				{ typedef char type; };

		template <>
			struct GetCharEncodingType<CharEncoding::locale_char>
				{ typedef char type; };

		template <>
			struct GetCharEncodingType<CharEncoding::locale_wchar_t>
				{ typedef wchar_t type; };

		template <>
			struct GetCharEncodingType<CharEncoding::ucs2be>
				{ typedef char16_t type; };

		template <>
			struct GetCharEncodingType<CharEncoding::ucs2le>
				{ typedef char16_t type; };

		template <>
			struct GetCharEncodingType<CharEncoding::utf8>
				{ typedef char type; };

		template <>
			struct GetCharEncodingType<CharEncoding::utf16be>
				{ typedef char16_t type; };

		template <>
			struct GetCharEncodingType<CharEncoding::utf16le>
				{ typedef char16_t type; };

		template <>
			struct GetCharEncodingType<CharEncoding::utf32be>
				{ typedef char32_t type; };

		template <>
			struct GetCharEncodingType<CharEncoding::utf32le>
				{ typedef char32_t type; };

		////////////////////////////////////////////////////////////////////////

		/**
		 * Resolves a @c CharEncoding for a data type.
		 */
		template <CharEncoding encoding, typename T>
			struct ResolveCharEncoding :
				std::integral_constant<CharEncoding, encoding>
		{
			static_assert(is_character<T>::value,
				"not a character type");

			static_assert(sizeof (T) >= sizeof (typename GetCharEncodingType<encoding>::type),
				"type too small to represent encoding");
		};

		template <>
			struct ResolveCharEncoding<CharEncoding::defaultForType, char> :
				std::integral_constant<CharEncoding, ResolveCharEncoding<CharEncoding::locale_char, char>::value> {};

		template <>
			struct ResolveCharEncoding<CharEncoding::defaultForType, wchar_t> :
				std::integral_constant<CharEncoding, ResolveCharEncoding<CharEncoding::locale_wchar_t, wchar_t>::value> {};

		template <>
			struct ResolveCharEncoding<CharEncoding::defaultForType, char16_t> :
				std::integral_constant<CharEncoding, ResolveCharEncoding<CharEncoding::utf16, char16_t>::value> {};

		template <>
			struct ResolveCharEncoding<CharEncoding::defaultForType, char32_t> :
				std::integral_constant<CharEncoding, ResolveCharEncoding<CharEncoding::utf32, char32_t>::value> {};

		template <>
			struct ResolveCharEncoding<CharEncoding::utf, wchar_t> :
				std::integral_constant<CharEncoding, ResolveCharEncoding<CharEncoding::utf16, wchar_t>::value> {};

		template <>
			struct ResolveCharEncoding<CharEncoding::utf, char16_t> :
				std::integral_constant<CharEncoding, ResolveCharEncoding<CharEncoding::utf16, char16_t>::value> {};

		template <>
			struct ResolveCharEncoding<CharEncoding::utf, char32_t> :
				std::integral_constant<CharEncoding, ResolveCharEncoding<CharEncoding::utf32, char32_t>::value> {};

		////////////////////////////////////////////////////////////////////////

		/**
		 * Returns the default @c CharEncoding for a data type.
		 */
		template <typename T>
			struct GetDefaultCharEncoding :
				std::integral_constant<CharEncoding, ResolveCharEncoding<CharEncoding::defaultForType, T>::value> {};
	}
}

#endif
