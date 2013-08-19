#include "../../../err/lua.hpp" // CheckError, luaL_dostring_unprotected
#include "Coroutine.hpp"

namespace page { namespace script { namespace lua { namespace lib
{
	// construct
	Coroutine::Coroutine(lua_State *state)
	{
		struct Protected
		{
			static int Call(lua_State *state)
			{
				lua_pop(state, 1);
				// execute Lua code
				luaL_dostring_unprotected(state,
					// method to access local variables and up-
					// values proposed by Chris Swetenham
					// http://lua-users.org/lists/lua-l/2008-02/msg00293.html
					"do\n"
					"	local m = {}\n"
					"	function m:__index(key)\n"
					"		for i, k, v in debug.locals(6) do\n"
					"			if k == key then return v end\n"
					"		end\n"
					"		for i, k, v in debug.upvalues(coroutine.wait) do\n"
					"			if k == key then return v end\n"
					"		end\n"
					"		local v = rawget(self, key)\n"
					"		if v ~= nil then return v end\n"
					"		return rawget(getfenv(0), key)\n"
					"	end\n"
					"	function m:__newindex(key, value)\n"
					"		for i, k, v in debug.locals(6) do\n"
					"			if k == key then\n"
					"				debug.setlocal(6, i, value)\n"
					"				return\n"
					"			end\n"
					"		end\n"
					"		for i, k, v in debug.upvalues(coroutine.wait) do\n"
					"			if k == key then\n"
					"				debug.setupvalue(coroutine.wait, i, value)\n"
					"				return\n"
					"			end\n"
					"		end\n"
					"		if rawget(self, key) then\n"
					"			rawset(self, key, value)\n"
					"			return\n"
					"		end\n"
					"		rawset(getfenv(0), key, value)\n"
					"	end\n"
					"	local e = setmetatable({}, m)\n"
					"\n"
					"	function coroutine.wait(condition)\n"
					"		condition = assert(loadstring('return ' .. condition))\n"
					"		setfenv(condition, e)\n"
					"		while not condition() do coroutine.yield() end\n"
					"	end\n"
					"end");
				return 0;
			}
		};
		err::lua::CheckError(state, lua_cpcall(state, Protected::Call, 0));
	}
}}}}
