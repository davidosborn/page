#ifndef    page_local_util_locale_Transcoder_hpp
#   define page_local_util_locale_Transcoder_hpp

#	include <locale> // codecvt

#	include "CharEncoding.hpp"
#	include "UsableFacet.hpp"

namespace page
{
	namespace util
	{
		/**
		 * Specifies the direction of a @c std::codecvt conversion.
		 */
		enum class Direction
		{
			multibyteToWide,
			wideToMultibyte
		};

		////////////////////////////////////////////////////////////////////////

		/**
		 * Converts characters from one encoding to another, using the specified
		 * @c std::codecvt facet and @c Direction.  If an intermediate
		 * transcoder is specified, it is used as an input for this transcoder.
		 */
		template <typename Codecvt, Direction direction,
			typename IntermediateTranscoder = void>
				struct Transcoder
		{
			private:
			typedef Transcoder<Codecvt, direction> FinalTranscoder;

			static_assert(
				std::is_same<
					typename FinalTranscoder::argument_type,
					typename IntermediateTranscoder::result_type>::value,
				"intermediate transcoder is not compatible");

			public:
			typedef typename FinalTranscoder::result_type result_type;
			typedef typename IntermediateTranscoder::argument_type argument_type;

			result_type operator ()(const argument_type &s) const
			{
				return FinalTranscoder()(IntermediateTranscoder()(s));
			}
		};

		template <typename Codecvt>
			struct Transcoder<Codecvt, Direction::multibyteToWide>
		{
			private:
			typedef typename Codecvt::intern_type   ToChar;
			typedef typename Codecvt::extern_type FromChar;

			public:
			typedef std::basic_string<  ToChar>   result_type;
			typedef std::basic_string<FromChar> argument_type;

			result_type operator ()(const argument_type &s) const
			{
				// FIXME: GCC doesn't have wstring_convert yet
				// FIXME: need to handle errors
				//return std::wstring_convert<Codecvt>().from_bytes(s);

				// FIXME: this implementation is so bad, but its temporary
				const FromChar *a_begin = &*s.begin();
				const FromChar *a_end   = &*s.end();
				const FromChar *a_next;
				std::basic_string<ToChar> r(1000, 0);
				ToChar *b_begin = &*r.begin();
				ToChar *b_end   = &*r.end();
				ToChar *b_next;
				std::mbstate_t mbstate;
				Codecvt().in(mbstate,
					a_begin, a_end, a_next,
					b_begin, b_end, b_next);
				return r;
			}
		};

		template <typename Codecvt>
			struct Transcoder<Codecvt, Direction::wideToMultibyte>
		{
			private:
			typedef typename Codecvt::extern_type   ToChar;
			typedef typename Codecvt::intern_type FromChar;

			public:
			typedef std::basic_string<  ToChar>   result_type;
			typedef std::basic_string<FromChar> argument_type;

			result_type operator ()(const argument_type &s) const
			{
				// FIXME: GCC doesn't have wstring_convert yet
				// FIXME: need to handle errors
				//return std::wstring_convert<Codecvt>().to_bytes(s);

				// FIXME: this implementation is so bad, but its temporary
				const FromChar *a_begin = &*s.begin();
				const FromChar *a_end   = &*s.end();
				const FromChar *a_next;
				std::basic_string<ToChar> r(1000, 0);
				ToChar *b_begin = &*r.begin();
				ToChar *b_end   = &*r.end();
				ToChar *b_next;
				std::mbstate_t mbstate;
				Codecvt().out(mbstate,
					a_begin, a_end, a_next,
					b_begin, b_end, b_next);
				return r;
			}
		};

		////////////////////////////////////////////////////////////////////////

		/**
		 * Returns a @c Transcoder that will perform a conversion from one
		 * character encoding to another.
		 */
		template <
			CharEncoding fromEncoding = CharEncoding::defaultForType,
			CharEncoding   toEncoding = CharEncoding::defaultForType>
				struct GetTranscoder;

		/**
		 * The identity conversion.
		 */
		template <CharEncoding encoding>
			struct GetTranscoder<encoding, encoding>
		{
			typedef Transcoder<
				UsableFacet<std::codecvt<char, char, std::mbstate_t>>,
				Direction::multibyteToWide> type;
		};

		/**
		 * Converts from char to wchar_t.
		 */
		template <>
			struct GetTranscoder<
				CharEncoding::locale_char,
				CharEncoding::locale_wchar_t>
		{
			typedef Transcoder<
				UsableFacet<std::codecvt<wchar_t, char, std::mbstate_t>>,
				Direction::multibyteToWide> type;
		};

		/**
		 * Converts from wchar_t to char.
		 */
		template <>
			struct GetTranscoder<
				CharEncoding::locale_wchar_t,
				CharEncoding::locale_char>
		{
			typedef Transcoder<
				UsableFacet<std::codecvt<wchar_t, char, std::mbstate_t>>,
				Direction::wideToMultibyte> type;
		};
	}
}

#endif
