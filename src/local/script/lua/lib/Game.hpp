#ifndef    page_local_script_lua_lib_Game_hpp
#   define page_local_script_lua_lib_Game_hpp

#	include <lua.hpp> // lua_State

namespace page { namespace script { namespace lua { namespace lib
{
	struct Game
	{
		// construct
		explicit Game(lua_State *);

		private:
		// functions
		static int Quit(lua_State *);
	};
}}}}

#endif
