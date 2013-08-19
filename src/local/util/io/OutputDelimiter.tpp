namespace page
{
	namespace util
	{
		/*--------------------------+
		| constructors & destructor |
		+--------------------------*/

		template <typename Char, typename CharTraits>
			OutputDelimiter<Char, CharTraits>::OutputDelimiter() {}

		template <typename Char, typename CharTraits>
			OutputDelimiter<Char, CharTraits>::OutputDelimiter(Char c) :
				string(1, c) {}

		template <typename Char, typename CharTraits>
			OutputDelimiter<Char, CharTraits>::OutputDelimiter(const Char *s) :
				string(s) {}

		template <typename Char, typename CharTraits>
			OutputDelimiter<Char, CharTraits>::OutputDelimiter(const String &s) :
				string(s) {}

		/*----------+
		| observers |
		+----------*/

		template <typename Char, typename CharTraits>
			const typename OutputDelimiter<Char, CharTraits>::String &
			OutputDelimiter<Char, CharTraits>::GetString() const
		{
			return string;
		}

		template <typename Char, typename CharTraits>
			OutputDelimiter<Char, CharTraits>::operator String() const
		{
			return string;
		}

		template <typename Char, typename CharTraits>
			OutputDelimiter<Char, CharTraits>::operator bool() const
		{
			return !string.empty();
		}

		/*----------------------+
		| predefined delimiters |
		+----------------------*/

		template <typename Char, typename CharTraits>
			const OutputDelimiter<Char, CharTraits> &
			OutputDelimiter<Char, CharTraits>::GetEmptyDelimiter()
		{
			return OutputDelimiter();
		}

		template <typename Char, typename CharTraits>
			const OutputDelimiter<Char, CharTraits> &
			OutputDelimiter<Char, CharTraits>::GetSpaceDelimiter()
		{
			static OutputDelimiter delimiter(CharTraits::to_char_type(' '));
			return delimiter;
		}

		/*------------------------------+
		| stream insertion & extraction |
		+------------------------------*/

		template <typename Char, typename CharTraits>
			std::basic_ostream<Char, CharTraits> &operator <<(std::basic_ostream<Char, CharTraits> &os, const OutputDelimiter<Char, CharTraits> &delimiter)
		{
			return os << delimiter.GetString();
		}
	}
}