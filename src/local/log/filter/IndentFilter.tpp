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
			IndentFilter::IndentFilter(
				const std::shared_ptr<IndentFilterState> &state,
				Branches &&... branches) :
					PrefixFilter(std::forward<Branches>(branches)...),
					state(state)
		{
			assert(state);
		}
	}
}
