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

#include <cassert>
#include <iostream> // cout

#include "../../../err/lua.hpp" // CATCH_LUA_ERRORS, CheckError, luaL_dostring_unprotected
#include "../../../util/pp.hpp" // STRINGIZE
#include "Base.hpp"

namespace page
{
	namespace script
	{
		namespace lua
		{
			namespace lib
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
			}
		}
	}
}
