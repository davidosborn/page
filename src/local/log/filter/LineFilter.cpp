#include <sstream> // [io]stringstream

#include "../../err/Exception.hpp"
#include "LineFilter.hpp"

namespace page
{
	namespace log
	{
		/*----------------------+
		| Filter implementation |
		+----------------------*/

		std::string LineFilter::DoFilter(const std::string &s) const
		{
			std::istringstream is(buffer + s);
			std::ostringstream os;
			std::string str;
			while (std::getline(is, str).good())
				os << FilterLine(str) << std::endl;
			buffer = str;
			if (is.fail() && !is.eof())
				THROW((err::Exception<err::LogModuleTag, err::StreamReadTag>()))
			if (os.fail())
				THROW((err::Exception<err::LogModuleTag, err::StreamWriteTag>()))
			return os.str();
		}

		/*----------------------+
		| Stream implementation |
		+----------------------*/

		void LineFilter::DoClear()
		{
			if (!buffer.empty())
			{
				Write('\n');
				buffer.clear();
			}
		}
	}
}
