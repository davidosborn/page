/**
 * @section license
 *
 * Copyright (c) 2006-2012 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution, and in the same
 *    place and form as other copyright, license, and disclaimer information.
 *
 * As a special exception, distributions of derivative works in binary form may
 * include an acknowledgement in place of the above copyright notice, this list
 * of conditions, and the following disclaimer in the documentation and/or other
 * materials provided with the distribution, and in the same place and form as
 * other acknowledgements, similar in substance to the following:
 *
 *    Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#include <iostream> // cout
#include "cfg.hpp" // Commit, Init
#include "err/exception/catch.hpp" // CATCH_ALL_AND_PRINT_ERROR_AND
#include "game/Game.hpp" // Game::{{,~}Game,Run}
#include "log/print.hpp" // Print{Info,Stats}
#include "opt.hpp" // Parse
#include "sys.hpp" // PrintInfo

namespace page
{
	namespace
	{
		struct Initer
		{
			Initer(int argc, char *argv[])
			{
				opt::Parse(argc, argv);
				sys::PrintInfo();
				cfg::Init();
				log::PrintInfo();
			}
			~Initer()
			{
				log::PrintStats();
			}
		};
	}
}

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
		Initer initer(argc, argv);
		try
		{
			game::Game().Run();
			cfg::Commit();
		}
		CATCH_ALL_AND_PRINT_ERROR_AND(status = 1;)
		std::cout << "exiting with status " << status << std::endl;
	}
	CATCH_ALL_AND_PRINT_ERROR_AND(status = 1;)
	return status;
}
