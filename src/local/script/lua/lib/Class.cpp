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

#include <cassert>
#include <vector>

#include "../../../err/lua.hpp" // CATCH_LUA_ERRORS, CheckError
#include "../../../util/cpp.hpp" // STRINGIZE
#include "Class.hpp"

namespace page { namespace script { namespace lua { namespace lib
{
	namespace
	{
		// C++/Lua instance mapping userdata
		typedef std::shared_ptr<void> InstanceUserdata;
		static int DeleteInstanceUserdata(lua_State *state)
		{
			InstanceUserdata *userdata =
				static_cast<InstanceUserdata *>(
					lua_touserdata(state, 1));
			lua_pop(state, 1);
			try
			{
				userdata->~InstanceUserdata();
			}
			CATCH_LUA_ERRORS(state)
			return 0;
		}
	}

	// construct
	Class::Class(lua_State *state)
	{
		struct Protected
		{
			static int Call(lua_State *state)
			{
				lua_pop(state, 1);
				// create class table
				luaL_dostring_unprotected(state,
					"class = {}\n"
					"setmetatable(class, class)\n"
					"\n"
					"function class:__index(c)\n"
					"	return function(...)\n"
					"		c = assert(loadstring(c .. ' = {}; return ' .. c))()\n"
					"		setmetatable(c, c)\n"
					"		local bases = {...}\n"
					"		if #bases == 1 then\n"
					"			local base = bases[1]\n"
					"			function c:__index(key)\n"
					"				local v = rawget(base, key)\n"
					"				if v ~= nil then return v end\n"
					"				local h = rawget(getmetatable(base) or {}, '__index')\n"
					"				if h == nil then return h end\n"
					"				if type(h) == 'function' then\n"
					"					return h(self, key)\n"
					"				else\n"
					"					return h[key]\n"
					"				end\n"
					"			end\n"
					"		elseif #bases > 1 then\n"
					"			function c:__index(key)\n"
					"				local v\n"
					"				for i, base in ipairs(bases) do\n"
					"					local v = rawget(base, key)\n"
					"					if v ~= nil then break end\n"
					"					local h = rawget(getmetatable(base) or {}, '__index')\n"
					"					if h == nil then break end\n"
					"					if type(h) == 'function' then\n"
					"						v = h(self, key)\n"
					"					else\n"
					"						v = h[key]\n"
					"					end\n"
					"					if v ~= nil then break end\n"
					"				end\n"
					"				return v\n"
					"			end\n"
					"		end\n"
					"		function c:__call(...)\n"
					"			local o = {}\n"
					"			setmetatable(o, o)\n"
					"			function o:__index(key)\n"
					"				local v = rawget(c, key)\n"
					"				if v ~= nil then return v end\n"
					"				local h = rawget(getmetatable(c) or {}, '__index')\n"
					"				if h == nil then return h end\n"
					"				if type(h) == 'function' then\n"
					"					return h(self, key)\n"
					"				else\n"
					"					return h[key]\n"
					"				end\n"
					"			end\n"
					"			function o:__newindex(key, value)\n"
					"				if rawget(c, key) ~= nil then\n"
					"					rawset(c, key, value)\n"
					"					return true\n"
					"				end\n"
					"				local h = rawget(getmetatable(c) or {}, '__newindex')\n"
					"				if h ~= nil then\n"
					"					if type(h) == 'function' then\n"
					"						if h(self, key, value) then return true end\n"
					"					else\n"
					"						h[key] = value\n"
					"						return true\n"
					"					end\n"
					"				end\n"
					"				rawset(self, key, value)\n"
					"				return true\n"
					"			end\n"
					"			if c.__init then c.__init(o, ...) end\n"
					"			return o\n"
					"		end\n"
					"		function c:__newindex(key, value)\n"
					"			for i, base in ipairs(bases) do\n"
					"				if rawget(base, key) ~= nil then\n"
					"					rawset(base, key, value)\n"
					"					return true\n"
					"				end\n"
					"				local h = rawget(getmetatable(base) or {}, '__newindex')\n"
					"				if h ~= nil then\n"
					"					if type(h) == 'function' then\n"
					"						if h(self, key, value) then return true end\n"
					"					else\n"
					"						h[key] = value\n"
					"						return true\n"
					"					end\n"
					"				end\n"
					"			end\n"
					"			rawset(self, key, value)\n"
					"			return true\n"
					"		end\n"
					"		return c\n"
					"	end\n"
					"end\n"
					"\n"
					"function chain_index(c, h)\n"
					"	local mt = getmetatable(c) or setmetatable(c, c)\n"
					"	local next_h = rawget(mt, '__index')\n"
					"	rawset(mt, '__index', function(self, key)\n"
					"		local v\n"
					"		if type(h) == 'function' then\n"
					"			v = h(self, key)\n"
					"		else\n"
					"			v = h[key]\n"
					"		end\n"
					"		if v ~= nil then return v end\n"
					"		if next_h then\n"
					"			if type(next_h) == 'function' then\n"
					"				return next_h(self, key)\n"
					"			else\n"
					"				return next_h[key]\n"
					"			end\n"
					"		end\n"
					"	end)\n"
					"end\n"
					"function chain_newindex(c, h)\n"
					"	local mt = getmetatable(c) or setmetatable(c, c)\n"
					"	local next_h = rawget(mt, '__newindex')\n"
					"	rawset(mt, '__newindex', function(self, key, value)\n"
					"		if type(h) == 'function' then\n"
					"			if h(self, key, value) then return true end\n"
					"		else\n"
					"			if h[key] ~= nil then\n"
					"				h[key] = value\n"
					"				return true\n"
					"			end\n"
					"		end\n"
					"		if next_h then\n"
					"			if type(next_h) == 'function' then\n"
					"				return next_h(self, key, value)\n"
					"			else\n"
					"				next_h[key] = value\n"
					"				return true\n"
					"			end\n"
					"		end\n"
					"	end)\n"
					"end\n"
					"\n"
					"function enable_properties(c)\n"
					"	if rawget(c, '__getters') == nil then rawset(c, '__getters', {}) end\n"
					"	if rawget(c, '__setters') == nil then rawset(c, '__setters', {}) end\n"
					"	chain_index(c, function(self, key)\n"
					"		local getter = rawget(c, '__getters')[key]\n"
					"		if getter ~= nil then\n"
					"			return getter(self)\n"
					"		end\n"
					"	end)\n"
					"	chain_newindex(c, function(self, key, value)\n"
					"		local setter = rawget(c, '__setters')[key]\n"
					"		if setter ~= nil then\n"
					"			setter(self, value)\n"
					"			return true\n"
					"		end\n"
					"	end)\n"
					"end");
				// create C++/Lua instance mapping table
				lua_pushstring(state, STRINGIZE(PACKAGE) ".lib.instance");
				lua_newtable(state);
				lua_newtable(state);
				lua_pushstring(state, "__mode");
				lua_pushstring(state, "k");
				lua_rawset(state, -3);
				lua_setmetatable(state, -2);
				lua_rawset(state, LUA_REGISTRYINDEX);
				// register functions
				luaL_Reg funcs[] =
				{
					{"enable_delete", &Class::EnableDelete},
					{}
				};
				luaL_register(state, "_G", funcs);
				lua_pop(state, 1);
				return 0;
			}
		};
		err::lua::CheckError(state, lua_cpcall(state, Protected::Call, 0));
	}

	// instance mapping
	void Class::RegisterInstance(lua_State *state, const std::shared_ptr<void> &instance)
	{
		luaL_argcheck(state, lua_istable(state, -1), 1, "table expected");
		lua_pushstring(state, STRINGIZE(PACKAGE) ".lib.instance");
		lua_rawget(state, LUA_REGISTRYINDEX);
		lua_pushvalue(state, -2);
		InstanceUserdata *userdata =
			static_cast<InstanceUserdata *>(
				lua_newuserdata(state, sizeof *userdata));
		try
		{
			new(userdata) InstanceUserdata(instance);
		}
		CATCH_LUA_ERRORS(state)
		lua_newtable(state);
		lua_pushstring(state, "__gc");
		lua_pushcfunction(state, DeleteInstanceUserdata);
		lua_rawset(state, -3);
		lua_setmetatable(state, -2);
		lua_rawset(state, -3);
		lua_pop(state, 2);
	}
	void Class::DeleteInstance(lua_State *state)
	{
		luaL_argcheck(state, lua_istable(state, -1), 1, "table expected");
		lua_pushstring(state, STRINGIZE(PACKAGE) ".lib.instance");
		lua_rawget(state, LUA_REGISTRYINDEX);
		lua_pushvalue(state, -2);
		lua_rawget(state, -2);
		lua_getmetatable(state, -1);
		lua_pushstring(state, "__gc");
		lua_rawget(state, -2);
		lua_pushvalue(state, -3);
		lua_call(state, 1, 0);
		lua_pop(state, 1);
		lua_pushnil(state);
		lua_setmetatable(state, -2);
		lua_pop(state, 1);
		lua_pushvalue(state, -2);
		lua_pushnil(state);
		lua_rawset(state, -3);
		lua_pop(state, 2);
	}
	void *Class::GetInstance(lua_State *state)
	{
		luaL_argcheck(state, lua_istable(state, -1), 1, "table expected");
		lua_pushstring(state, STRINGIZE(PACKAGE) ".lib.instance");
		lua_rawget(state, LUA_REGISTRYINDEX);
		lua_pushvalue(state, -2);
		lua_rawget(state, -2);
		InstanceUserdata *userdata =
			static_cast<InstanceUserdata *>(
				lua_touserdata(state, -1));
		lua_pop(state, 3);
		return userdata ? userdata->get() : 0;
	}

	// property registration
	void Class::RegisterProperties(lua_State *state, const char *name, const Property *properties)
	{
		assert(name);
		std::vector<luaL_Reg> getters, setters;
		try
		{
			for (const Property *property = properties; property->name; ++property)
			{
				if (property->getter)
				{
					luaL_Reg getter =
					{
						property->name,
						property->getter
					};
					getters.push_back(getter);
				}
				if (property->setter)
				{
					luaL_Reg setter =
					{
						property->name,
						property->setter
					};
					setters.push_back(setter);
				}
			}
			luaL_Reg sentry = {};
			getters.push_back(sentry);
			setters.push_back(sentry);
		}
		CATCH_LUA_ERRORS(state)
		lua_getglobal(state, name);
		lua_getfield(state, -1, "__getters");
		luaL_register(state, 0, &*getters.begin());
		lua_pop(state, 1);
		lua_getfield(state, -1, "__setters");
		luaL_register(state, 0, &*setters.begin());
		lua_pop(state, 2);
	}

	// functions
	int Class::EnableDelete(lua_State *state)
	{
		struct Router
		{
			static int Delete(lua_State *state)
			{
				Class::DeleteInstance(state);
				return 0;
			}
		};
		luaL_argcheck(state, lua_istable(state, 1), 1, "table expected");
		lua_pushcfunction(state, &Router::Delete);
		lua_setfield(state, -2, "delete");
		lua_pop(state, 1);
		return 0;
	}
}}}}
