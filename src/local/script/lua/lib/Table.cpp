/**
 * @section license
 *
 * Copyright (c) 2006-2013 David Osborn
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

#include "../../../err/lua.hpp" // CheckError, luaL_dostring_unprotected
#include "Table.hpp"

namespace page { namespace script { namespace lua { namespace lib
{
	// construct
	Table::Table(lua_State *state)
	{
		struct Protected
		{
			static int Call(lua_State *state)
			{
				lua_pop(state, 1);
				// execute Lua code
				luaL_dostring_unprotected(state,
					"function table.tostring(t)\n"
					"	local function key(k)\n"
					"		return\n"
					"			type(k) ~= 'number' and\n"
					"			type(k) ~= 'string' and '[' .. tostring(k) .. ']' or k\n"
					"	end\n"
					"	local function value(v)\n"
					"		if type(v) == 'string' then\n"
					"			v = string.gsub(v, '\\a', '\\\\a')\n"
					"			v = string.gsub(v, '\\b', '\\\\b')\n"
					"			v = string.gsub(v, '\\f', '\\\\f')\n"
					"			v = string.gsub(v, '\\n', '\\\\n')\n"
					"			v = string.gsub(v, '\\r', '\\\\r')\n"
					"			v = string.gsub(v, '\\t', '\\\\t')\n"
					"			v = string.gsub(v, '\\v', '\\\\v')\n"
					"			v = string.gsub(v, '\\\\', '\\\\\\\\')\n"
					"			v = string.gsub(v, \"'\", \"\\\\'\")\n"
					"			return \"'\" .. v .. \"'\"\n"
					"		end\n"
					"		return type(v) == 'table' and table.tostring(v) or tostring(v)\n"
					"	end\n"
					"	local s = '{'\n"
					"	local k, v = next(t)\n"
					"	if v ~= nil then\n"
					"		if k == 1 then\n"
					"			s = s .. value(v)\n"
					"			local next = ipairs(t)\n"
					"			for i, v in next, t, k do\n"
					"				s = s .. ',' .. value(v)\n"
					"				k = i\n"
					"			end\n"
					"		else\n"
					"			s = s .. key(k) .. '=' .. value(v)\n"
					"		end\n"
					"		for k, v in next, t, k do\n"
					"			s = s .. ',' .. key(k) .. '=' .. value(v)\n"
					"		end\n"
					"	end\n"
					"	return s .. '}'\n"
					"end");
				return 0;
			}
		};
		err::lua::CheckError(state, lua_cpcall(state, Protected::Call, 0));
	}
}}}}
