#ifndef    page_local_script_lua_lib_Character_hpp
#   define page_local_script_lua_lib_Character_hpp

#	include <lua.hpp> // lua_State

namespace page { namespace script { namespace lua { namespace lib
{
	struct Character
	{
		// construct
		explicit Character(lua_State *);

		private:
		// functions
		static int Get(lua_State *);
		static int Init(lua_State *);

		// actions
		static int Cheer(lua_State *);
		static int Clap(lua_State *);
		static int Dance(lua_State *);
		static int Goto(lua_State *);
		static int Jump(lua_State *);
		static int Run(lua_State *);
		static int RunTo(lua_State *);
		static int Say(lua_State *);
		static int Sleep(lua_State *);
		static int Walk(lua_State *);
		static int WalkTo(lua_State *);

		// properties
		static int GetAwake(lua_State *);
		static int SetAwake(lua_State *);
		static int GetEmotion(lua_State *);
		static int SetEmotion(lua_State *);

		// conditions
		static int IsIdle(lua_State *);
		static int IsMoving(lua_State *);
		static int IsRunning(lua_State *);
		static int IsSpeaking(lua_State *);
		static int IsWalking(lua_State *);
	};
}}}}

#endif
