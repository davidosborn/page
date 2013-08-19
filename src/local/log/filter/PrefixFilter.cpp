#include <cstring> // memchr

#include "PrefixFilter.hpp"

namespace page
{
	namespace log
	{
		/*--------------------------+
		| LineFilter implementation |
		+--------------------------*/

		std::string PrefixFilter::FilterLine(const std::string &line) const
		{
			return GetPrefix() + line;
		}
	}
}
