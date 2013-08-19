#include <cassert>

#include "Filter.hpp"

namespace page
{
	namespace log
	{
		/*----------------------+
		| Stream implementation |
		+----------------------*/

		void Filter::DoWrite(const std::string &s)
		{
			auto f = DoFilter(s);
			if (!f.empty())
				BranchableStream::DoWrite(f);
		}
	}
}
