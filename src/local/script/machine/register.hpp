#ifndef    page_local_script_machine_register_hpp
#   define page_local_script_machine_register_hpp

#	include "../../res/type/script/ScriptFormat.hpp"
#	include "registry.hpp" // MachineFactory, RegisterMachine

namespace page
{
	namespace script
	{
#	define REGISTER_MACHINE(factory, format) \
		namespace \
		{ \
			MachineRegisterer _regMachine(factory, format); \
		}

		struct MachineRegisterer
		{
			MachineRegisterer(const MachineFactory &factory, res::ScriptFormat format)
				{ RegisterMachine(factory, format); }
		};
	}
}

#endif
