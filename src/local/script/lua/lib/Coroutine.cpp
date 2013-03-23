/**
 * @section license
 *
 * Copyright (c) 2006-2013 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution, and in the same
 *    place and form as other copyright, license, and disclaimer information.
 *
 * As a special exception, distributions of derivative works in binary form may
 * include an acknowledgement in place of the above copyright notice, this list
 * of conditions, and the following disclaimer in the documentation and/or other
 * materials provided with the distribution, and in the same place and form as
 * other acknowledgements, similar in substance to the following:
 *
 *    Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

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
