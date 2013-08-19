#include "../../../err/lua.hpp" // CATCH_LUA_ERRORS, CheckError
#include "../../Router.hpp" // Router::Quit
#include "../Library.hpp" // GetLibrary, Library::router
#include "Game.hpp"

namespace page { namespace script { namespace lua { namespace lib
{
	// construct
	Game::Game(lua_State *state)
	{
		struct Protected
		{
			static int Call(lua_State *state)
			{
				lua_pop(state, 1);
				// register functions
				luaL_Reg funcs[] =
				{
					{"quit", &Game::Quit},
					{}
				};
				luaL_register(state, "_G", funcs);
				lua_pop(state, 1);
				return 0;
			}
		};
		err::lua::CheckError(state, lua_cpcall(state, Protected::Call, 0));
	}

	// functions
	int Game::Quit(lua_State *state)
	{
		try
		{
			GetLibrary(state).router.Quit();
		}
		CATCH_LUA_ERRORS(state)
		return 0;
	}
}}}}
