#ifndef    page_local_script_lua_lib_Entity_hpp
#   define page_local_script_lua_lib_Entity_hpp

#	include <lua.hpp> // lua_State

namespace page { namespace script { namespace lua { namespace lib
{
	struct Entity
	{
		// construct
		explicit Entity(lua_State *);

		private:
		// actions
		static int Do(lua_State *);

		// properties
		static int GetPosition(lua_State *);
		static int SetPosition(lua_State *);
		static int GetOrientation(lua_State *);
		static int SetOrientation(lua_State *);
		static int GetScale(lua_State *);
		static int SetScale(lua_State *);
	};
}}}}

#endif
