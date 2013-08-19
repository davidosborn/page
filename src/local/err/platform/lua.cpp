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
