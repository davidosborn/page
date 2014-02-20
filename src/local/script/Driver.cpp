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

#include <cassert>

#include "../res/type/Script.hpp" // Script::format
#include "Driver.hpp"
#include "Machine.hpp" // Machine::Open
#include "machine/registry.hpp" // MakeMachine
#include "Process.hpp" // Process::Continue

namespace page
{
	namespace script
	{
		// construct
		Driver::Driver(game::Game &game) : router(game)
		{
			// FIXME: implement
		}

		// update
		void Driver::Update(float deltaTime)
		{
			// FIXME: if a process is blocked by a condition, don't update until
			// the condition has been met
			for (Processes::iterator iter(processes.begin()); iter != processes.end();)
			{
				Process &process(**iter);
				if (!process.Continue(deltaTime))
					iter = processes.erase(iter);
				else ++iter;
			}
		}

		// execution
		void Driver::Run(const res::Script &script)
		{
			// find compatible machine
			Machines::iterator iter(machines.find(script.format));
			if (iter == machines.end())
				iter = machines.insert(std::make_pair(script.format,
					std::shared_ptr<Machine>(MakeMachine(script.format, router)))).first;
			Machine &machine(*iter->second);
			// initiate script as new process on machine
			processes.push_back(std::shared_ptr<Process>(machine.Open(script)));
			assert(processes.back());
		}
	}
}
