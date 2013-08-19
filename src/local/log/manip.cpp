#include <ostream>
#include <typeinfo> // bad_cast

#include "filter/IndentFilterState.hpp"
#include "stream/Stream.hpp" // Stream::Clear

namespace page
{
	namespace log
	{
		/*------+
		| lines |
		+------*/

		std::ostream &Clear(std::ostream &os)
		{
			try
			{
				dynamic_cast<Stream &>(*os.rdbuf()).Clear();
			}
			catch (const std::bad_cast &) {}
			return os;
		}

		/*------------+
		| indentation |
		+------------*/

		std::ostream &Indent(std::ostream &os)
		{
			GLOBAL(IndentFilterState).Indent();
			return os;
		}

		std::ostream &Dedent(std::ostream &os)
		{
			GLOBAL(IndentFilterState).Dedent();
			return os;
		}

		/*---------------+
		| classification |
		+---------------*/

		std::ostream &Error(std::ostream &os)
		{
			return os << Clear << "error: ";
		}

		std::ostream &Warning(std::ostream &os)
		{
			return os << Clear << "warning: ";
		}
	}
}
