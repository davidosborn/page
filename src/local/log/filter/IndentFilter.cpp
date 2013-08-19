#include "IndentFilter.hpp"

namespace page
{
	namespace log
	{
		/*----------------------------+
		| PrefixFilter implementation |
		+----------------------------*/

		std::string IndentFilter::GetPrefix() const
		{
			return std::string(state->GetLevel() * 2, ' ');
		}
	}
}
