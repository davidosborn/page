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

#include "../../../err/lua.hpp" // CATCH_LUA_ERRORS, CheckError, luaL_dostring_unprotected
#include "../../../game/Entity.hpp" // Entity::{Get,Set}{Position,Orientation,Scale}
#include "../../../math/Euler.hpp"
#include "../../../math/Quat.hpp"
#include "../../../math/Vector.hpp"
#include "../../../util/platform/lua.hpp" // Get{Euler,{,Horizontal}Vector}, Push
#include "Class.hpp" // Class::{GetInstance,RegisterProperties}
#include "Entity.hpp"

namespace page { namespace script { namespace lua { namespace lib
{
	// construct
	Entity::Entity(lua_State *state)
	{
		struct Protected
		{
			static int Call(lua_State *state)
			{
				lua_pop(state, 1);
				// execute Lua code
				luaL_dostring_unprotected(state,
					"class.Entity()\n"
					"enable_properties(Entity)");
				// register functions
				luaL_Reg funcs[] =
				{
					{"do", &Entity::Do},
					{}
				};
				luaL_register(state, "_G", funcs);
				lua_pop(state, 1);
				// register properties
				Class::Property properties[] =
				{
					{"position",    &Entity::GetPosition,    &Entity::SetPosition},
					{"orientation", &Entity::GetOrientation, &Entity::SetOrientation},
					{"scale",       &Entity::GetScale,       &Entity::SetScale},
					{}
				};
				Class::RegisterProperties(state, "Entity", properties);
				return 0;
			}
		};
		err::lua::CheckError(state, lua_cpcall(state, Protected::Call, 0));
	}

	// actions
	int Entity::Do(lua_State *state)
	{
		lua_settop(state, 2);
		// extract instance argument
		luaL_argcheck(state, lua_istable(state, 1), 1, "table expected");
		game::Entity *entity = static_cast<game::Entity *>(Class::GetInstance(state));
		if (!entity) return 0;
		// extract path argument
		luaL_argcheck(state, lua_isstring(state, -1), 1, "string expected");
		std::string path;
		try
		{
			path = lua_tostring(state, -1);
		}
		CATCH_LUA_ERRORS(state)
		lua_pop(state, 1);
		// apply animation
		try
		{
			// FIXME: is this the right solution?
			/*Library &lib(GetLibrary(state));
			lib.router.ApplyAnimation(entity, path);*/
		}
		CATCH_LUA_ERRORS(state)
		return 0;
	}

	// properties
	int Entity::GetPosition(lua_State *state)
	{
		lua_settop(state, 1);
		// extract instance argument
		luaL_argcheck(state, lua_istable(state, 1), 1, "table expected");
		game::Entity *entity = static_cast<game::Entity *>(Class::GetInstance(state));
		if (!entity) return 0;
		// load return value
		math::Vec3 position;
		try
		{
			position = entity->GetPosition();
		}
		CATCH_LUA_ERRORS(state)
		util::lua::Push(state, position);
		return 1;
	}
	int Entity::SetPosition(lua_State *state)
	{
		lua_settop(state, 2);
		// extract instance argument
		luaL_argcheck(state, lua_istable(state, 1), 1, "table expected");
		lua_pushvalue(state, 1);
		lua_remove(state, 1);
		game::Entity *entity = static_cast<game::Entity *>(Class::GetInstance(state));
		if (!entity) return 0;
		// extract value argument
		std::pair<math::Vec3, bool> value(
			util::lua::GetHorizontalVector(state));
		if (value.second) entity->SetPosition(value.first);
		else entity->SetPosition(Swizzle(value.first, 0, 2));
		return 0;
	}
	int Entity::GetOrientation(lua_State *state)
	{
		lua_settop(state, 1);
		// extract instance argument
		luaL_argcheck(state, lua_istable(state, 1), 1, "table expected");
		game::Entity *entity = static_cast<game::Entity *>(Class::GetInstance(state));
		if (!entity) return 0;
		// load return value
		math::Euler<> orientation;
		try
		{
			orientation = RadToDeg(math::Euler<>(entity->GetOrientation()));
		}
		CATCH_LUA_ERRORS(state)
		util::lua::Push(state, orientation);
		return 1;
	}
	int Entity::SetOrientation(lua_State *state)
	{
		lua_settop(state, 2);
		// extract instance argument
		luaL_argcheck(state, lua_istable(state, 1), 1, "table expected");
		lua_pushvalue(state, 1);
		lua_remove(state, 1);
		game::Entity *entity = static_cast<game::Entity *>(Class::GetInstance(state));
		if (!entity) return 0;
		// extract value argument
		math::Euler<> orientation(util::lua::GetEuler(state));
		try
		{
			entity->SetOrientation(math::Quat<>(DegToRad(orientation)));
		}
		CATCH_LUA_ERRORS(state)
		return 0;
	}
	int Entity::GetScale(lua_State *state)
	{
		lua_settop(state, 1);
		// extract instance argument
		luaL_argcheck(state, lua_istable(state, 1), 1, "table expected");
		game::Entity *entity = static_cast<game::Entity *>(Class::GetInstance(state));
		if (!entity) return 0;
		// load return value
		math::Vec3 scale;
		try
		{
			scale = entity->GetScale();
		}
		CATCH_LUA_ERRORS(state)
		util::lua::Push(state, scale);
		return 1;
	}
	int Entity::SetScale(lua_State *state)
	{
		lua_settop(state, 2);
		// extract instance argument
		luaL_argcheck(state, lua_istable(state, 1), 1, "table expected");
		lua_pushvalue(state, 1);
		lua_remove(state, 1);
		game::Entity *entity = static_cast<game::Entity *>(Class::GetInstance(state));
		if (!entity) return 0;
		// extract value argument
		math::Vec3 scale(util::lua::GetVector(state));
		try
		{
			entity->SetScale(scale);
		}
		CATCH_LUA_ERRORS(state)
		return 0;
	}
}}}}
