#ifndef    page_local_util_locale_convert_hpp
#   define page_local_util_locale_convert_hpp

#	include <string> // allocator, basic_string, char_traits

#	include "CharEncoding.hpp"

namespace page { namespace util
{
	/**
	 * Converts a string to a different character encoding.
	 *
	 * Characters that cannot be represented in the new encoding are dropped.
	 *
	 * @throw err::Exception<err::UtilModuleTag, err::ConversionTag>
	 */
	template <
		CharEncoding   ToCharEncoding,
		CharEncoding FromCharEncoding = CharEncoding::defaultForType,
		typename       ToChar       = typename GetCharEncodingType<ToCharEncoding>::type,
		typename       ToCharTraits = std::char_traits<ToChar>,
		typename       ToAllocator  = std::allocator<ToChar>,
		typename     FromChar,
		typename     FromCharTraits = std::char_traits<FromChar>,
		typename     FromAllocator  = std::allocator<FromChar>>
			std::basic_string<ToChar, ToCharTraits, ToAllocator>
			Convert(const std::basic_string<FromChar, FromCharTraits, FromAllocator> &);

	/**
	 * @copydoc Convert
	 */
	template <
		CharEncoding   ToCharEncoding,
		CharEncoding FromCharEncoding = CharEncoding::defaultForType,
		typename       ToChar       = typename GetCharEncodingType<ToCharEncoding>::type,
		typename       ToCharTraits = std::char_traits<ToChar>,
		typename       ToAllocator  = std::allocator<ToChar>,
		typename     FromChar,
		typename     FromCharTraits = std::char_traits<FromChar>,
		typename     FromAllocator  = std::allocator<FromChar>>
			std::basic_string<ToChar, ToCharTraits, ToAllocator>
			Convert(const FromChar *);
}}

#	include "convert.tpp"
#endif
