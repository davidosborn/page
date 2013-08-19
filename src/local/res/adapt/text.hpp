#ifndef    page_local_res_adapt_text_hpp
#   define page_local_res_adapt_text_hpp

#	include <string>

namespace page
{
	namespace res
	{
		class Stream;

		bool CheckSig(Stream &, const std::string &sig);
	}
}

#endif
