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

#include "../../err/lua.hpp" // CheckError
#include "../../util/pp.hpp" // STRINGIZE
#include "Library.hpp"

namespace page
{
	namespace script
	{
		namespace lua
		{
			// construct
			Library::Library(lua_State *state, Router &router) :
				standard(state),
				base(state), coroutine(state), debug(state), table(state),
				_class(state),
				game(state),
				entity(state), character(state), object(state),
				router(router)
			{
				struct Protected
				{
					static int Call(lua_State *state)
					{
						Library *_this = static_cast<Library *>(lua_touserdata(state, 1));
						lua_pop(state, 1);
						lua_pushstring(state, STRINGIZE(PACKAGE) ".lib");
						lua_pushlightuserdata(state, _this);
						lua_rawset(state, LUA_REGISTRYINDEX);
						return 0;
					}
				};
				err::lua::CheckError(state, lua_cpcall(state, Protected::Call, this));
			}

			// associative access
			Library &GetLibrary(lua_State *state)
			{
				struct Protected
				{
					static int Call(lua_State *state)
					{
						Library **result = static_cast<Library **>(lua_touserdata(state, 1));
						lua_pop(state, 1);
						lua_pushstring(state, STRINGIZE(PACKAGE) ".lib");
						lua_rawget(state, LUA_REGISTRYINDEX);
						*result = static_cast<Library *>(lua_touserdata(state, -1));
						lua_pop(state, 1);
						return 0;
					}
				};
				Library *result;
				err::lua::CheckError(state, lua_cpcall(state, Protected::Call, &result));
				return *result;
			}
		}
	}
}
