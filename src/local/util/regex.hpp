#ifndef    page_local_util_regex_hpp
#   define page_local_util_regex_hpp

#	include <istream>
#	include <ostream>
#	include <regex>

namespace page
{
	namespace util
	{
		// stream insertion/extraction
		template <typename SC, typename ST, typename RC, typename RT>
			std::basic_ostream<SC, ST> &operator <<(std::basic_ostream<SC, ST> &, const std::basic_regex<RC, RT> &);
		template <typename SC, typename ST, typename RC, typename RT>
			std::basic_istream<SC, ST> &operator >>(std::basic_istream<SC, ST> &, std::basic_regex<RC, RT> &);
	}
}

#	include "regex.tpp"
#endif
