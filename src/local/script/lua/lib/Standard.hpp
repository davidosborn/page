#ifndef    page_local_script_lua_lib_Standard_hpp
#   define page_local_script_lua_lib_Standard_hpp

#	include <lua.hpp> // lua_State

namespace page { namespace script { namespace lua { namespace lib
{
	struct Standard
	{
		// construct
		explicit Standard(lua_State *);
	};
}}}}

#endif
