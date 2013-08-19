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
