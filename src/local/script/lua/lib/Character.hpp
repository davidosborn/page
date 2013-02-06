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

#ifndef    page_local_script_lua_lib_Character_hpp
#   define page_local_script_lua_lib_Character_hpp

#	include <lua.hpp> // lua_State

namespace page
{
	namespace script
	{
		namespace lua
		{
			namespace lib
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
			}
		}
	}
}

#endif
