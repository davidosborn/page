#include <X11/Xlib.h>

#include "../../err/Exception.hpp"
#include "../ConsoleRegistry.hpp" // REGISTER_CONSOLE
#include "Console.hpp"

namespace page { namespace wnd { namespace x11
{
	Console::Console(const std::string &title)
	{
		// FIXME: implement
	}

	void Console::Put(char c)
	{
		// FIXME: implement
	}

	void Console::Put(const std::string &s)
	{
		// FIXME: implement
	}

	REGISTER_CONSOLE(Console, "X11 console")
}}}
