#ifndef    page_local_script_lua_lib_Object_hpp
#   define page_local_script_lua_lib_Object_hpp

#	include <lua.hpp> // lua_State

namespace page { namespace script { namespace lua { namespace lib
{
	struct Object
	{
		// construct
		explicit Object(lua_State *);

		private:
		// functions
		static int Init(lua_State *);
		static int Get(lua_State *);
	};
}}}}

#endif
