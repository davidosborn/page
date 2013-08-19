#include <cassert>
#include <iostream> // cout

#include "../../../err/lua.hpp" // CATCH_LUA_ERRORS, CheckError, luaL_dostring_unprotected
#include "../../../util/cpp.hpp" // STRINGIZE
#include "Base.hpp"

namespace page { namespace script { namespace lua { namespace lib
{
	// construct
	Base::Base(lua_State *state)
	{
		struct Protected
		{
			static int Call(lua_State *state)
			{
				lua_pop(state, 1);
				// execute Lua code
				luaL_dostring_unprotected(state,
					"function delay(duration)\n"
					"	local target = os.clock() + duration;\n"
					"	while os.clock() < target do coroutine.yield() end\n"
					"end");
				// register functions
				luaL_Reg funcs[] =
				{
					{"fork",  &Base::Fork},
					{"print", &Base::Print},
					{}
				};
				luaL_register(state, "_G", funcs);
				lua_pop(state, 1);
				// set global variables
				lua_pushliteral(state, LUA_VERSION " for " STRINGIZE(NAME VERSION));
				lua_setglobal(state, "_VERSION");
				return 0;
			}
		};
		err::lua::CheckError(state, lua_cpcall(state, Protected::Call, 0));
	}

	// functions
	int Base::Fork(lua_State *state)
	{
		// FIXME: implement
		return 0;
	}
	int Base::Print(lua_State *state)
	{
		if (lua_gettop(state)) for (;;)
		{
			lua_getfield(state, LUA_GLOBALSINDEX, "tostring");
			lua_pushvalue(state, 1);
			lua_call(state, 1, 1);
			try
			{
				std::cout << lua_tostring(state, -1);
			}
			CATCH_LUA_ERRORS(state)
			lua_pop(state, 1);
			lua_remove(state, 1);
			if (!lua_gettop(state)) break;
			try
			{
				std::cout << "\t";
			}
			CATCH_LUA_ERRORS(state)
		}
		try
		{
			std::cout << std::endl;
		}
		CATCH_LUA_ERRORS(state)
		return 0;
	}
}}}}
