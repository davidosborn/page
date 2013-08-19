#include "../../err/Exception.hpp"
#include "../../util/functional/factory.hpp" // new_function
#include "../machine/register.hpp" // REGISTER_MACHINE
#include "Library.hpp"
#include "Machine.hpp"

namespace page
{
	namespace script
	{
		namespace lua
		{
			// construct/destroy
			Machine::Machine(Router &router)
			{
				if (!(state = luaL_newstate()))
					THROW((err::Exception<err::ScriptModuleTag, err::LuaPlatformTag>("failed to initialize interpreter")))
				lib.reset(new Library(state, router));
			}
			Machine::~Machine()
			{
				lib.reset();
				lua_close(state);
			}

			// execution
			Process *Machine::Open(const res::Script &script)
			{
				return new Process(state, script);
			}
		}

		REGISTER_MACHINE((util::new_function<lua::Machine, Router &>()), res::luaScriptFormat)
	}
}
