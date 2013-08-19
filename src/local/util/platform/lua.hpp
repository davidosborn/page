#ifndef    page_local_util_platform_lua_hpp
#   define page_local_util_platform_lua_hpp

#	include <utility>

#	include <lua.hpp>

#	include "../../math/fwd.hpp" // Euler, Vector

namespace page
{
	namespace util
	{
		namespace lua
		{
			// extraction
			math::Euler<> GetEuler(lua_State *);
			std::pair<math::Vec3, bool> GetHorizontalVector(lua_State *);
			math::Vec3 GetVector(lua_State *);

			// insertion
			void Push(lua_State *, const math::Euler<> &);
			void Push(lua_State *, const math::Vec2 &);
			void Push(lua_State *, const math::Vec3 &);
		}
	}
}

#endif
