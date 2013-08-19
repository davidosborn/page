#ifndef    page_local_script_lua_lib_Table_hpp
#   define page_local_script_lua_lib_Table_hpp

#	include <lua.hpp> // lua_State

namespace page { namespace script { namespace lua { namespace lib
{
	struct Table
	{
		// construct
		explicit Table(lua_State *);
	};
}}}}

#endif
