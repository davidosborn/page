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

#ifndef    page_err_lua_hpp
#   define page_err_lua_hpp

#	include <lua.hpp> // lua_State, luaL_error
#	include "exception/catch.hpp" // CATCH_ALL_AND

namespace page
{
	namespace err
	{
		namespace lua
		{
			void CheckError(lua_State *, int);
			void CheckErrorForLua(lua_State *, int);
		}
	}
}

#	define CATCH_LUA_ERRORS(state) \
		CATCH_ALL_AND(::luaL_error(state, CAUGHT_EXCEPTION_WHAT);)

#	define luaL_dofile_unprotected(state, file) \
		::page::err::lua::CheckErrorForLua(state, ::luaL_loadfile(state, file)); \
		::lua_call(state, 0, 0);
#	define luaL_dostring_unprotected(state, string) \
		::page::err::lua::CheckErrorForLua(state, ::luaL_loadstring(state, string)); \
		::lua_call(state, 0, 0);

#endif
