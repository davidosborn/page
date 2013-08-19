#ifndef    page_local_script_machine_registry_hpp
#   define page_local_script_machine_registry_hpp

#	include <functional> // function

#	include "../../res/type/script/ScriptFormat.hpp"

namespace page
{
	namespace script
	{
		class Machine;
		class Router;

		// factory type
		typedef std::function<Machine *(Router &)> MachineFactory;

		// creation
		Machine *MakeMachine(res::ScriptFormat, Router &);

		// registration
		void RegisterMachine(const MachineFactory &, res::ScriptFormat);
	}
}

#endif
