#include <string>

#include "../../util/string/operations.hpp" // TrimLeading
#include "../pipe/Stream.hpp"

namespace page
{
	namespace res
	{
		bool CheckSig(Stream &stream, const std::string &sig)
		{
			std::string line(util::TrimLeading(stream.GetLine()));
			return line.substr(0, sig.size()) == sig;
		}
	}
}
