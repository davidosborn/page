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
