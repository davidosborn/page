/**
 * @copyright
 *
 * Copyright (c) 2006-2014 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the same place
 *    and form as other copyright, license, and disclaimer information.
 *
 * 3. Redistributions in binary form must also include an acknowledgement in the
 *    same place and form as other acknowledgements (such as the credits),
 *    similar in substance to the following:
 *
 *       Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

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
