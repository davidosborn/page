/**
 * @copyright
 *
 * Copyright (c) 2006-2014 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the same place
 *    and form as other copyright, license, and disclaimer information.
 *
 * 3. Redistributions in binary form must also include an acknowledgement in the
 *    same place and form as other acknowledgements (such as the credits),
 *    similar in substance to the following:
 *
 *       Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#include <iostream> // cout

#include "cfg/CmdlineParser.hpp"
#include "cfg/state/State.hpp"
#include "err/report.hpp" // ReportError, std::exception
#include "game/Game.hpp" // Game::{{,~}Game,Run}
#include "log/print.hpp" // Print{Info,Stats}
#include "sys/info.hpp" // PrintInfo

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
		GLOBAL(cfg::CmdlineParser).Parse(argc, argv);
		sys::PrintInfo();
		log::PrintInfo();

		game::Game().Run();

		GLOBAL(cfg::State).Commit();
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
