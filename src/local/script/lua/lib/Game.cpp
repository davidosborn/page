/**
 * @copyright
 *
 * Copyright (c) 2006-2014 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the same place
 *    and form as other copyright, license, and disclaimer information.
 *
 * 3. Redistributions in binary form must also include an acknowledgement in the
 *    same place and form as other acknowledgements (such as the credits),
 *    similar in substance to the following:
 *
 *       Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

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
