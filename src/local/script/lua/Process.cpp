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

#include "../../err/lua.hpp" // CheckError
#include "../../res/type/Script.hpp" // Script::text
#include "Process.hpp"

namespace page
{
	namespace script
	{
		namespace lua
		{
			// construct/destroy
			Process::Process(lua_State *state, const res::Script &script)
			{
				struct Protected
				{
					static int Call1(lua_State *state)
					{
						Process *_this = static_cast<Process *>(lua_touserdata(state, 1));
						lua_pop(state, 1);
						_this->state = lua_newthread(state);
						_this->thread = luaL_ref(state, LUA_REGISTRYINDEX);
						return 0;
					}
					static int Call2(lua_State *state)
					{
						Process *_this = static_cast<Process *>(lua_touserdata(state, 1));
						lua_pop(state, 1);
						// HACK: depends on buffer underflow semantics
						lua_insert(state, -2);
						_this->coroutine = luaL_ref(state, LUA_REGISTRYINDEX);
						return 0;
					}
				};
				err::lua::CheckError(state, lua_cpcall(state, Protected::Call1, this));
				err::lua::CheckError(this->state, luaL_loadstring(this->state, script.text.c_str()));
				err::lua::CheckError(this->state, lua_cpcall(this->state, Protected::Call2, this));
			}
			Process::~Process()
			{
				luaL_unref(state, LUA_REGISTRYINDEX, coroutine);
				luaL_unref(state, LUA_REGISTRYINDEX, thread);
			}

			// execution
			bool Process::Continue(float deltaTime)
			{
				lua_rawgeti(state, LUA_REGISTRYINDEX, coroutine);
				int result = lua_resume(state, 0);
				err::lua::CheckError(state, result);
				return result;
			}
		}
	}
}
