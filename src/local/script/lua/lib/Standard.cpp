#include "../../../err/lua.hpp" // CheckError
#include "Standard.hpp"

namespace page { namespace script { namespace lua { namespace lib
{
	// construct
	Standard::Standard(lua_State *state)
	{
		struct Protected
		{
			static int Call(lua_State *state)
			{
				lua_pop(state, 1);
				luaL_openlibs(state);
				return 0;
			}
		};
		err::lua::CheckError(state, lua_cpcall(state, Protected::Call, 0));
	}
}}}}
