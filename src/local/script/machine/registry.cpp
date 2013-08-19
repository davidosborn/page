#include <cassert>
#include <unordered_map>

#include "../../err/Exception.hpp"
#include "registry.hpp" // MachineFactory

namespace page
{
	namespace script
	{
		namespace
		{
			struct Registry
			{
				typedef std::unordered_map<res::ScriptFormat, MachineFactory, std::hash<int>> Factories;
				Factories factories;
			};
			inline Registry &GetRegistry()
			{
				static Registry reg;
				return reg;
			}
		}

		// creation
		Machine *MakeMachine(res::ScriptFormat format, Router &router)
		{
			const Registry &reg(GetRegistry());
			Registry::Factories::const_iterator iter(reg.factories.find(format));
			if (iter != reg.factories.end()) return iter->second(router);
			THROW((err::Exception<err::ScriptModuleTag, err::NotAvailableTag>("no " + GetName(format) + " machine available")))
		}

		// registration
		void RegisterMachine(const MachineFactory &factory, res::ScriptFormat format)
		{
			assert(factory);
			GetRegistry().factories.insert(std::make_pair(format, factory));
		}
	}
}
