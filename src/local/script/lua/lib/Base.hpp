#ifndef    page_local_script_lua_lib_Base_hpp
#   define page_local_script_lua_lib_Base_hpp

#	include <lua.hpp> // lua_State

namespace page { namespace script { namespace lua { namespace lib
{
	struct Base
	{
		// construct
		explicit Base(lua_State *);

		private:
		// functions
		static int Fork(lua_State *);
		static int Print(lua_State *);
	};
}}}}

#endif
