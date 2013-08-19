#include "../../err/lua.hpp" // CheckError
#include "../../util/cpp.hpp" // STRINGIZE
#include "Library.hpp"

namespace page
{
	namespace script
	{
		namespace lua
		{
			// construct
			Library::Library(lua_State *state, Router &router) :
				standard(state),
				base(state), coroutine(state), debug(state), table(state),
				_class(state),
				game(state),
				entity(state), character(state), object(state),
				router(router)
			{
				struct Protected
				{
					static int Call(lua_State *state)
					{
						Library *_this = static_cast<Library *>(lua_touserdata(state, 1));
						lua_pop(state, 1);
						lua_pushstring(state, STRINGIZE(PACKAGE) ".lib");
						lua_pushlightuserdata(state, _this);
						lua_rawset(state, LUA_REGISTRYINDEX);
						return 0;
					}
				};
				err::lua::CheckError(state, lua_cpcall(state, Protected::Call, this));
			}

			// associative access
			Library &GetLibrary(lua_State *state)
			{
				struct Protected
				{
					static int Call(lua_State *state)
					{
						Library **result = static_cast<Library **>(lua_touserdata(state, 1));
						lua_pop(state, 1);
						lua_pushstring(state, STRINGIZE(PACKAGE) ".lib");
						lua_rawget(state, LUA_REGISTRYINDEX);
						*result = static_cast<Library *>(lua_touserdata(state, -1));
						lua_pop(state, 1);
						return 0;
					}
				};
				Library *result;
				err::lua::CheckError(state, lua_cpcall(state, Protected::Call, &result));
				return *result;
			}
		}
	}
}
