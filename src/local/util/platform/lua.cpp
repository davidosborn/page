#include <utility> // pair

#include <lua.hpp>

#include "../../math/Euler.hpp"
#include "../../math/Vector.hpp"

namespace page
{
	namespace util
	{
		namespace lua
		{
			// extraction
			math::Euler<> GetEuler(lua_State *state)
			{
				math::Euler<> result;
				if (!lua_istable(state, -1))
				{
					luaL_argcheck(state, lua_isnumber(state, -1), 2, "table or number expected");
					result.yaw = lua_tonumber(state, -1);
					lua_pop(state, 1);
				}
				else switch (lua_objlen(state, -1))
				{
					case 0: luaL_argerror(state, 2, "table of length 1 to 3 expected");
					case 1:
					lua_pushinteger(state, 1);
					lua_rawget(state, -2);
					result.yaw = lua_tonumber(state, -1);
					lua_pop(state, 2);
					break;
					case 2:
					lua_pushinteger(state, 1);
					lua_rawget(state, -2);
					result.yaw = lua_tonumber(state, -1);
					lua_pop(state, 1);
					lua_pushinteger(state, 2);
					lua_rawget(state, -2);
					result.pitch = lua_tonumber(state, -1);
					lua_pop(state, 2);
					break;
					default:
					lua_pushinteger(state, 1);
					lua_rawget(state, -2);
					result.yaw = lua_tonumber(state, -1);
					lua_pop(state, 1);
					lua_pushinteger(state, 2);
					lua_rawget(state, -2);
					result.pitch = lua_tonumber(state, -1);
					lua_pop(state, 1);
					lua_pushinteger(state, 3);
					lua_rawget(state, -2);
					result.roll = lua_tonumber(state, -1);
					lua_pop(state, 2);
				}
				return result;
			}
			std::pair<math::Vec3, bool> GetHorizontalVector(lua_State *state)
			{
				std::pair<math::Vec3, bool> result(0, true);
				if (!lua_istable(state, -1))
				{
					luaL_argcheck(state, lua_isnumber(state, -1), 2, "table or number expected");
					result.first = lua_tonumber(state, -1);
					lua_pop(state, 1);
				}
				else switch (lua_objlen(state, -1))
				{
					case 0: luaL_argerror(state, 2, "table of length 1 to 3 expected");
					case 1:
					lua_pushinteger(state, 1);
					lua_rawget(state, -2);
					result.first = lua_tonumber(state, -1);
					lua_pop(state, 2);
					break;
					case 2:
					lua_pushinteger(state, 1);
					lua_rawget(state, -2);
					result.first.x = lua_tonumber(state, -1);
					lua_pop(state, 1);
					lua_pushinteger(state, 2);
					lua_rawget(state, -2);
					result.first.z = lua_tonumber(state, -1);
					lua_pop(state, 2);
					result.second = false;
					break;
					default:
					lua_pushinteger(state, 1);
					lua_rawget(state, -2);
					result.first.x = lua_tonumber(state, -1);
					lua_pop(state, 1);
					lua_pushinteger(state, 2);
					lua_rawget(state, -2);
					result.first.y = lua_tonumber(state, -1);
					lua_pop(state, 1);
					lua_pushinteger(state, 3);
					lua_rawget(state, -2);
					result.first.z = lua_tonumber(state, -1);
					lua_pop(state, 2);
				}
				return result;
			}
			math::Vec3 GetVector(lua_State *state)
			{
				math::Vec3 result;
				if (!lua_istable(state, -1))
				{
					luaL_argcheck(state, lua_isnumber(state, -1), 2, "table or number expected");
					result = lua_tonumber(state, -1);
					lua_pop(state, 1);
				}
				else switch (lua_objlen(state, -1))
				{
					case 0:
					case 2: luaL_argerror(state, 2, "table of length 1 or 3 expected");
					case 1:
					lua_pushinteger(state, 1);
					lua_rawget(state, -2);
					result = lua_tonumber(state, -1);
					lua_pop(state, 2);
					break;
					default:
					lua_pushinteger(state, 1);
					lua_rawget(state, -2);
					result.x = lua_tonumber(state, -1);
					lua_pop(state, 1);
					lua_pushinteger(state, 2);
					lua_rawget(state, -2);
					result.y = lua_tonumber(state, -1);
					lua_pop(state, 1);
					lua_pushinteger(state, 3);
					lua_rawget(state, -2);
					result.z = lua_tonumber(state, -1);
					lua_pop(state, 2);
				}
				return result;
			}

			// insertion
			void Push(lua_State *state, const math::Euler<> &value)
			{
				lua_createtable(state, 3, 0);
				lua_pushinteger(state, 1);
				lua_pushnumber(state, value.yaw);
				lua_rawset(state, -3);
				lua_pushinteger(state, 2);
				lua_pushnumber(state, value.pitch);
				lua_rawset(state, -3);
				lua_pushinteger(state, 3);
				lua_pushnumber(state, value.roll);
				lua_rawset(state, -3);
			}
			void Push(lua_State *state, const math::Vec2 &value)
			{
				lua_createtable(state, 2, 0);
				lua_pushinteger(state, 1);
				lua_pushnumber(state, value.x);
				lua_rawset(state, -3);
				lua_pushinteger(state, 2);
				lua_pushnumber(state, value.y);
				lua_rawset(state, -3);
			}
			void Push(lua_State *state, const math::Vec3 &value)
			{
				lua_createtable(state, 3, 0);
				lua_pushinteger(state, 1);
				lua_pushnumber(state, value.x);
				lua_rawset(state, -3);
				lua_pushinteger(state, 2);
				lua_pushnumber(state, value.y);
				lua_rawset(state, -3);
				lua_pushinteger(state, 3);
				lua_pushnumber(state, value.z);
				lua_rawset(state, -3);
			}
		}
	}
}
