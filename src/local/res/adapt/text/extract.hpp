#ifndef    page_local_res_adapt_text_extract_hpp
#   define page_local_res_adapt_text_extract_hpp

#	include <string>

namespace page
{
	namespace res
	{
		template <typename OutputIterator> void Extract(const std::string &, OutputIterator first, OutputIterator last);
		template <typename Container> void Extract(const std::string &, Container &);
	}
}

#	include "extract.tpp"
#endif
