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
