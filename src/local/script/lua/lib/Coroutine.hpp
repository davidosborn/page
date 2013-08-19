#ifndef    page_local_script_lua_lib_Coroutine_hpp
#   define page_local_script_lua_lib_Coroutine_hpp

#	include <lua.hpp> // lua_State

namespace page { namespace script { namespace lua { namespace lib
{
	struct Coroutine
	{
		// construct
		explicit Coroutine(lua_State *);
	};
}}}}

#endif
