#ifndef    page_local_script_lua_Machine_hpp
#   define page_local_script_lua_Machine_hpp

#	include <memory> // unique_ptr

#	include <lua.hpp> // luaState

#	include "../Machine.hpp"
#	include "Process.hpp" // Process->script::Process

namespace page
{
	namespace script
	{
		class Router;

		namespace lua
		{
			class Library;

			struct Machine : script::Machine
			{
				// construct/destroy
				explicit Machine(Router &);
				~Machine();

				// execution
				Process *Open(const res::Script &);

				private:
				lua_State *state;
				std::unique_ptr<Library> lib;
			};
		}
	}
}

#endif
