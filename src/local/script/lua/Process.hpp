#ifndef    page_local_script_lua_Process_hpp
#   define page_local_script_lua_Process_hpp

#	include <lua.hpp> // lua_State

#	include "../Process.hpp"

namespace page
{
	namespace res { class Script; }

	namespace script
	{
		namespace lua
		{
			struct Process : script::Process
			{
				// construct/destroy
				Process(lua_State *, const res::Script &);
				~Process();

				// execution
				bool Continue(float deltaTime);

				private:
				lua_State *state;
				int thread, coroutine;
			};
		}
	}
}

#endif
