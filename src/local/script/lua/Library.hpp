#ifndef    page_local_script_lua_Library_hpp
#   define page_local_script_lua_Library_hpp

#	include <lua.hpp> // lua_State

#	include "../../util/class/special_member_functions.hpp" // Uncopyable
#	include "lib/Base.hpp"
#	include "lib/Character.hpp"
#	include "lib/Class.hpp"
#	include "lib/Coroutine.hpp"
#	include "lib/Debug.hpp"
#	include "lib/Entity.hpp"
#	include "lib/Game.hpp"
#	include "lib/Object.hpp"
#	include "lib/Standard.hpp"
#	include "lib/Table.hpp"

namespace page
{
	namespace script
	{
		class Router;

		namespace lua
		{
			struct Library : util::Uncopyable<Library>
			{
				// construct
				Library(lua_State *, Router &);

				// modules
				lib::Standard  standard;
				// standard library extensions
				lib::Base      base;
				lib::Coroutine coroutine;
				lib::Debug     debug;
				lib::Table     table;
				// non-standard library extensions
				lib::Class     _class;
				// top-level game interfaces
				lib::Game      game;
				// entity class heirarchy
				lib::Entity    entity;
				lib::Character character;
				lib::Object    object;

				Router &router;
			};

			// associative access
			Library &GetLibrary(lua_State *);
		}
	}
}

#endif
