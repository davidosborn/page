namespace page
{
	namespace util
	{
		// stream insertion/extraction
		template <typename SC, typename ST, typename RC, typename RT>
			std::basic_ostream<SC, ST> &operator <<(std::basic_ostream<SC, ST> &os, const std::basic_regex<RC, RT> &re)
		{
			os << std::string(re);
		}
		template <typename SC, typename ST, typename RC, typename RT>
			std::basic_istream<SC, ST> &operator >>(std::basic_istream<SC, ST> &is, std::basic_regex<RC, RT> &re)
		{
			std::string s;
			is >> s;
			re = std::basic_regex<RC, RT>(s);
		}
	}
}
