#ifndef    page_local_util_string_StringBuilder_hpp
#   define page_local_util_string_StringBuilder_hpp

#	include <sstream> // basic_{ostringstream,string}

namespace page { namespace util
{
	/**
	 * An alternative to std::basic_ostringstream, which allows you to write
	 * one-liners, since the insertion operator returns the complete type.
	 */
	template <
		typename Char,
		typename CharTraits = std::char_traits<Char>>
			class BasicStringBuilder
	{
		public:
		typedef std::basic_string<Char, CharTraits> String;

		template <typename T>
			BasicStringBuilder &operator <<(T &&);

		operator String() const;

		private:
		std::basic_ostringstream<Char, CharTraits> ss;
	};

	typedef BasicStringBuilder<char>    StringBuilder;
	typedef BasicStringBuilder<wchar_t> WideStringBuilder;
}}

#	include "StringBuilder.tpp"
#endif
