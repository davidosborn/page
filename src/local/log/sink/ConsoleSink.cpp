#include "../../wnd/Console.hpp"
#include "../../wnd/ConsoleRegistry.hpp"
#include "ConsoleSink.hpp"

namespace page
{
	namespace log
	{
		/*--------------------------+
		| constructors & destructor |
		+--------------------------*/

		ConsoleSink::ConsoleSink(const std::string &title) :
			console(GLOBAL(wnd::ConsoleRegistry).Make(title)) {}

		/*----------------------+
		| Stream implementation |
		+----------------------*/

		void ConsoleSink::DoWrite(const std::string &s)
		{
			console->Write(s);
		}
	}
}
