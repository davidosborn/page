/**
 * @section license
 *
 * Copyright (c) 2006-2012 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.

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
#include "Debug.hpp"

namespace page
{
	namespace script
	{
		namespace lua
		{
			namespace lib
			{
				// construct
				Debug::Debug(lua_State *state)
				{
					struct Protected
					{
						static int Call(lua_State *state)
						{
							lua_pop(state, 1);
							// execute Lua code
							luaL_dostring_unprotected(state,
								"function debug.nextlocal(thread, level, index)\n"
								"	if level == nil then\n"
								"		thread, level = level, thread\n"
								"	end\n"
								"	index = index == nil and 1 or index + 1\n"
								"	local k, v\n"
								"	if thread ~= nil then\n"
								"		k, v = debug.getlocal(thread, level, index)\n"
								"	else\n"
								"		k, v = debug.getlocal(level, index)\n"
								"	end\n"
								"	if k == nil then return nil end\n"
								"	return index, k, v\n"
								"end\n"
								"function debug.locals(thread, level)\n"
								"	if level == nil then\n"
								"		thread, level = level, thread\n"
								"	end\n"
								"	return function(level, index)\n"
								"		return debug.nextlocal(thread, level, index)\n"
								"	end, level, nil\n"
								"end\n"
								"function debug.getlocals(thread, level)\n"
								"	if level == nil then\n"
								"		thread, level = level, thread\n"
								"	end\n"
								"	local r = {}\n"
								"	for i, k, v in debug.locals(thread, level) do\n"
								"		rawset(r, k, v)\n"
								"	end\n"
								"	return r\n"
								"end\n"
								"\n"
								"function debug.nextupvalue(func, index)\n"
								"	index = index == nil and 1 or index + 1\n"
								"	local k, v = debug.getupvalue(func, index)\n"
								"	if k == nil then return nil end\n"
								"	return index, k, v\n"
								"end\n"
								"function debug.upvalues(func)\n"
								"	return debug.nextupvalue, func, nil\n"
								"end\n"
								"function debug.getupvalues(func)\n"
								"	local r = {}\n"
								"	for i, k, v in debug.upvalues(func) do\n"
								"		rawset(r, k, v)\n"
								"	end\n"
								"	return r\n"
								"end");
							return 0;
						}
					};
					err::lua::CheckError(state, lua_cpcall(state, Protected::Call, 0));
				}
			}
		}
	}
}
