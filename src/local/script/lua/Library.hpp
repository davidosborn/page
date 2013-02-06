/**
 * @section license
 *
 * Copyright (c) 2006-2012 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution, and in the same
 *    place and form as other copyright, license, and disclaimer information.
 *
 * As a special exception, distributions of derivative works in binary form may
 * include an acknowledgement in place of the above copyright notice, this list
 * of conditions, and the following disclaimer in the documentation and/or other
 * materials provided with the distribution, and in the same place and form as
 * other acknowledgements, similar in substance to the following:
 *
 *    Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#ifndef    page_local_script_lua_Library_hpp
#   define page_local_script_lua_Library_hpp

#	include <lua.hpp> // lua_State
#	include "../../util/NonCopyable.hpp"
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
			struct Library : util::NonCopyable
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
