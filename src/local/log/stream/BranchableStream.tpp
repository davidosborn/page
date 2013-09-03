#include <cassert>
#include <utility> // forward

namespace page
{
	namespace log
	{
		/*-------------+
		| constructors |
		+-------------*/

		template <typename... Branches>
			BranchableStream::BranchableStream(Branches &&... branches) :
				branches{std::forward<Branches>(branches)...}
		{
			for (const auto &branch : this->branches)
				assert(branch);
		}
	}
}
