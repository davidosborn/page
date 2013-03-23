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

#include <cassert>
#include <string>

#include "../Exception.hpp"
#include "lua.hpp" // CATCH_LUA_ERRORS

namespace page { namespace err { namespace lua
{
	void CheckError(lua_State *state, int code)
	{
		switch (code)
		{
			case 0:
			case LUA_YIELD: return;
		}
		struct Protected
		{
			static int Call(lua_State *state)
			{
				std::string *msg = static_cast<std::string *>(lua_touserdata(state, 1));
				lua_pop(state, 1);
				try
				{
					// HACK: depends on buffer underflow semantics
					*msg = lua_tostring(state, -2);
				}
				CATCH_LUA_ERRORS(state)
				return 0;
			}
		};
		std::string msg;
		if (!lua_gettop(state) ||
			!lua_isstring(state, -1) ||
			lua_cpcall(state, Protected::Call, &msg))
		{
			switch (code)
			{
				case LUA_ERRRUN:    msg = "runtime error";        break;
				case LUA_ERRSYNTAX: msg = "syntax error";         break;
				case LUA_ERRMEM:    msg = "out of memory";        break;
				case LUA_ERRERR:    msg = "error handling error"; break;
				default: assert(!"invalid error code");
			}
		}
		THROW((err::Exception<err::ErrModuleTag, err::LuaPlatformTag>(msg)))
	}

	void CheckErrorForLua(lua_State *state, int code)
	{
		switch (code)
		{
			case 0:
			case LUA_YIELD: return;
		}
		if (lua_gettop(state)) lua_error(state);
		const char *msg;
		switch (code)
		{
			case LUA_ERRRUN:    msg = "runtime error";        break;
			case LUA_ERRSYNTAX: msg = "syntax error";         break;
			case LUA_ERRMEM:    msg = "out of memory";        break;
			case LUA_ERRERR:    msg = "error handling error"; break;
			default: assert(!"invalid error code");
		}
		luaL_error(state, msg);
	}
}}}
