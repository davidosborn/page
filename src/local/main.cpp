#include <iostream> // cout

#include "cfg/State.hpp"
#include "err/report.hpp" // ReportError, std::exception
#include "game/Game.hpp" // Game::{{,~}Game,Run}
#include "log/print.hpp" // Print{Info,Stats}
#include "opt.hpp" // Parse
#include "sys.hpp" // PrintInfo

#ifdef USE_WIN32
#	include <windows.h>
#	define argc __argc
#	define argv __argv
int WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
#else
int main(int argc, char *argv[])
#endif
{
	using namespace page;
	int status = 0;
	try
	{
		opt::Parse(argc, argv);
		sys::PrintInfo();
		log::PrintInfo();

		game::Game().Run();

		GLOBAL(cfg::State).Write();
		log::PrintStats();
	}
	catch (const std::exception &e)
	{
		err::ReportError(e);
		status = 1;
	}
	std::cout << "exiting with status " << status << std::endl;
	return status;
}
