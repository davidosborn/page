#include <locale>

#include "Transcoder.hpp"

namespace page
{
	namespace util
	{
		template <
			CharEncoding   ToCharEncoding,
			CharEncoding FromCharEncoding,
			typename   ToChar, typename   ToCharTraits, typename   ToAllocator,
			typename FromChar, typename FromCharTraits, typename FromAllocator>
				std::basic_string<ToChar, ToCharTraits, ToAllocator>
				Convert(const std::basic_string<FromChar, FromCharTraits, FromAllocator> &s)
		{
			return typename GetTranscoder<
				ResolveCharEncoding<FromCharEncoding, FromChar>::value,
				ResolveCharEncoding<  ToCharEncoding,   ToChar>::value>::type()(s);
		}

		template <
			CharEncoding   ToCharEncoding,
			CharEncoding FromCharEncoding,
			typename   ToChar, typename   ToCharTraits, typename   ToAllocator,
			typename FromChar, typename FromCharTraits, typename FromAllocator>
				std::basic_string<ToChar, ToCharTraits, ToAllocator>
				Convert(const FromChar *s)
		{
			return Convert<ToCharEncoding, FromCharEncoding, ToChar, ToCharTraits, ToAllocator>(
				std::basic_string<FromChar, FromCharTraits, FromAllocator>(s));
		}
	}
}
