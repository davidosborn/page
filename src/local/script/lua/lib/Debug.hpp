#ifndef    page_local_script_lua_lib_Debug_hpp
#   define page_local_script_lua_lib_Debug_hpp

#	include <functional> // function
#	include <memory> // shared_ptr
#	include <lua.hpp> // lua_State

namespace page { namespace script { namespace lua { namespace lib
{
	struct Debug
	{
		// construct
		explicit Debug(lua_State *);
	};
}}}}

#endif
