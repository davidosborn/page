#ifndef    page_local_err_platform_lua_hpp
#   define page_local_err_platform_lua_hpp

#	include <exception>

#	include <lua.hpp> // lua_State, luaL_error

namespace page { namespace err { namespace lua
{
	void CheckError(lua_State *, int);
	void CheckErrorForLua(lua_State *, int);
}}}

#	define CATCH_LUA_ERRORS(state) \
		catch (const std::exception &e) \
		{ \
			::luaL_error(state, e.what()); \
		}

#	define luaL_dofile_unprotected(state, file) \
		::page::err::lua::CheckErrorForLua(state, ::luaL_loadfile(state, file)); \
		::lua_call(state, 0, 0);

#	define luaL_dostring_unprotected(state, string) \
		::page::err::lua::CheckErrorForLua(state, ::luaL_loadstring(state, string)); \
		::lua_call(state, 0, 0);

#endif
