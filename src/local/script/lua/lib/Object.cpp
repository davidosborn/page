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

#include <functional> // bind
#include <memory> // shared_ptr
#include <string>

#include "../../../err/lua.hpp" // CATCH_LUA_ERRORS, CheckError, luaL_dostring_unprotected
#include "../../../game/Object.hpp"
#include "../../Router.hpp" // Router::{MakeObject,Remove}
#include "../Library.hpp" // GetLibrary, Library::router
#include "Class.hpp" // Class::RegisterInstance
#include "Object.hpp"

namespace page { namespace script { namespace lua { namespace lib
{
	// construct
	Object::Object(lua_State *state)
	{
		struct Protected
		{
			static int Call(lua_State *state)
			{
				lua_pop(state, 1);
				// create object table
				luaL_dostring_unprotected(state,
					"class.Object(Entity)\n"
					"enable_delete(Object)\n"
					"enable_properties(Object)");
				// register functions
				luaL_Reg funcs[] =
				{
					{"__init", &Object::Init},
					{"get",    &Object::Get},
					{}
				};
				luaL_register(state, "Object", funcs);
				lua_pop(state, 1);
				return 0;
			}
		};
		err::lua::CheckError(state, lua_cpcall(state, Protected::Call, 0));
	}

	// functions
	int Object::Get(lua_State *state)
	{
		// FIXME: implement
		return 1;
	}
	int Object::Init(lua_State *state)
	{
		lua_settop(state, 2);
		luaL_argcheck(state, lua_istable(state, 1), 1, "table expected");
		luaL_argcheck(state, lua_isstring(state, 2), 2, "string expected");
		// extract path argument
		std::string path;
		try
		{
			path = lua_tostring(state, -1);
		}
		CATCH_LUA_ERRORS(state)
		lua_pop(state, 1);
		// create instance
		std::shared_ptr<game::Object> object;
		try
		{
			Library &lib(GetLibrary(state));
			object = lib.router.MakeObject(path);
			try
			{
				object.reset(object.get(), std::bind(
					static_cast<void (Router::*)(const std::shared_ptr<game::Object> &)>(&Router::Remove),
					&lib.router, object));
			}
			catch (...)
			{
				lib.router.Remove(object);
				throw;
			}
		}
		CATCH_LUA_ERRORS(state)
		// register instance
		Class::RegisterInstance(state, object);
		return 0;
	}
}}}}
