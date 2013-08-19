#include <windows.h>

#include "../../err/Exception.hpp"
#include "../../util/locale/convert.hpp" // Convert
#include "../ConsoleRegistry.hpp" // REGISTER_CONSOLE
#include "Console.hpp"

namespace page { namespace wnd { namespace win32
{
	Console::Console(const std::string &title)
	{
		if (!AllocConsole() || !SetConsoleTitle(util::Convert<TCHAR>(title).c_str()))
			THROW((err::Exception<err::EnvModuleTag, err::Win32PlatformTag>("failed to allocate console") <<
				boost::errinfo_api_function("AllocConsole")))
	}

	void Console::Put(char c)
	{
		TCHAR tc = c;
		DWORD w;
		if (!WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), &tc, 1, &w, 0))
			THROW((err::Exception<err::EnvModuleTag, err::Win32PlatformTag>("failed to write to console") <<
				boost::errinfo_api_function("WriteConsole")))
	}

	void Console::Put(const std::string &s)
	{
		auto ts(util::Convert<TCHAR>(s));
		DWORD w;
		if (!WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), ts.c_str(), ts.size(), &w, 0))
			THROW((err::Exception<err::EnvModuleTag, err::Win32PlatformTag>("failed to write to console") <<
				boost::errinfo_api_function("WriteConsole")))
	}

	REGISTER_CONSOLE(Console, "Win32 console")
}}}
