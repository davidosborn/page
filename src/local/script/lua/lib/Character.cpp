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

#include <functional> // bind
#include <memory> // shared_ptr
#include <string>
#include "../../../err/lua.hpp" // CATCH_LUA_ERRORS, CheckError, luaL_dostring_unprotected
#include "../../../game/Character.hpp" // Character::{Goto,Is{Idle,Moving},Say}
#include "../../Router.hpp" // Router::{MakeCharacter,Remove}
#include "../Library.hpp" // GetLibrary, Library::router
#include "Character.hpp"
#include "Class.hpp" // Class::Register{Instance,Properties}

namespace page
{
	namespace script
	{
		namespace lua
		{
			namespace lib
			{
				// construct
				Character::Character(lua_State *state)
				{
					struct Protected
					{
						static int Call(lua_State *state)
						{
							lua_pop(state, 1);
							// create character table
							luaL_dostring_unprotected(state,
								"class.Character(Entity)\n"
								"enable_delete(Character)\n"
								"enable_properties(Character)");
							// register functions
							luaL_Reg funcs[] =
							{
								{"__init", &Character::Init},
								{"get",    &Character::Get},
								// actions
								{"cheer",  &Character::Cheer},
								{"clap",   &Character::Clap},
								{"dance",  &Character::Dance},
								{"goto",   &Character::Goto},
								{"jump",   &Character::Jump},
								{"run",    &Character::Run},
								{"runto",  &Character::RunTo},
								{"say",    &Character::Say},
								{"sleep",  &Character::Sleep},
								{"walk",   &Character::Walk},
								{"walkto", &Character::WalkTo},
								{}
							};
							luaL_register(state, "Character", funcs);
							lua_pop(state, 1);
							// register properties
							Class::Property properties[] =
							{
								{"awake",   &Character::GetAwake,   &Character::SetAwake},
								{"emotion", &Character::GetEmotion, &Character::SetEmotion},
								// conditions
								{"idle",     &Character::IsIdle},
								{"moving",   &Character::IsMoving},
								{"running",  &Character::IsRunning},
								{"speaking", &Character::IsSpeaking},
								{"walking",  &Character::IsWalking},
								{}
							};
							Class::RegisterProperties(state, "Character", properties);
							return 0;
						}
					};
					err::lua::CheckError(state, lua_cpcall(state, Protected::Call, 0));
				}

				// functions
				int Character::Get(lua_State *state)
				{
					// FIXME: implement
					return 1;
				}
				int Character::Init(lua_State *state)
				{
					lua_settop(state, 2);
					luaL_argcheck(state, lua_istable(state, 1), 1, "table expected");
					luaL_argcheck(state, lua_isstring(state, 2), 2, "string expected");
					// extract path argument
					std::string path;
					try
					{
						path = lua_tostring(state, -1);
					}
					CATCH_LUA_ERRORS(state)
					lua_pop(state, 1);
					// create instance
					std::shared_ptr<game::Character> character;
					try
					{
						Library &lib(GetLibrary(state));
						character = lib.router.MakeCharacter(path);
						try
						{
							character.reset(character.get(), std::bind(
								static_cast<void (Router::*)(const std::shared_ptr<game::Character> &)>(&Router::Remove),
								&lib.router, character));
						}
						catch (...)
						{
							lib.router.Remove(character);
							throw;
						}
					}
					CATCH_LUA_ERRORS(state)
					// register instance
					Class::RegisterInstance(state, character);
					return 0;
				}

				// actions
				int Character::Cheer(lua_State *state)
				{
					// FIXME: implement
					return 0;
				}
				int Character::Clap(lua_State *state)
				{
					// FIXME: implement
					return 0;
				}
				int Character::Dance(lua_State *state)
				{
					// FIXME: implement
					return 0;
				}
				int Character::Goto(lua_State *state)
				{
					// FIXME: implement
					return 0;
				}
				int Character::Jump(lua_State *state)
				{
					// FIXME: implement
					return 0;
				}
				int Character::Run(lua_State *state)
				{
					// FIXME: implement
					return 0;
				}
				int Character::RunTo(lua_State *state)
				{
					// FIXME: implement
					return 0;
				}
				int Character::Say(lua_State *state)
				{
					lua_settop(state, 2);
					luaL_argcheck(state, lua_istable(state, 1), 1, "table expected");
					luaL_argcheck(state, lua_isstring(state, 2), 2, "string expected");
					// extract speech argument
					std::string speech;
					try
					{
						speech = lua_tostring(state, -1);
					}
					CATCH_LUA_ERRORS(state)
					lua_pop(state, 1);
					// extract character argument
					game::Character *character = static_cast<game::Character *>(Class::GetInstance(state));
					if (character) character->Say(speech);
					return 0;
				}
				int Character::Sleep(lua_State *state)
				{
					// FIXME: implement
					return 0;
				}
				int Character::Walk(lua_State *state)
				{
					// FIXME: implement
					return 0;
				}
				int Character::WalkTo(lua_State *state)
				{
					// FIXME: implement
					return 0;
				}

				// properties
				int Character::GetAwake(lua_State *state)
				{
					// FIXME: implement
					lua_pushboolean(state, true);
					return 1;
				}
				int Character::SetAwake(lua_State *state)
				{
					// FIXME: implement
					return 0;
				}
				int Character::GetEmotion(lua_State *state)
				{
					// FIXME: implement
					lua_pushstring(state, "none");
					return 1;
				}
				int Character::SetEmotion(lua_State *state)
				{
					// FIXME: implement
					return 0;
				}

				// conditions
				int Character::IsIdle(lua_State *state)
				{
					game::Character *character = static_cast<game::Character *>(Class::GetInstance(state));
					if (!character) return 0;
					lua_pushboolean(state, character->IsIdle());
					return 1;
				}
				int Character::IsMoving(lua_State *state)
				{
					game::Character *character = static_cast<game::Character *>(Class::GetInstance(state));
					if (!character) return 0;
					lua_pushboolean(state, character->IsMoving());
					return 1;
				}
				int Character::IsRunning(lua_State *state)
				{
					// FIXME: implement
					lua_pushboolean(state, false);
					return 1;
				}
				int Character::IsSpeaking(lua_State *state)
				{
					// FIXME: implement
					lua_pushboolean(state, false);
					return 1;
				}
				int Character::IsWalking(lua_State *state)
				{
					// FIXME: implement
					lua_pushboolean(state, false);
					return 1;
				}
			}
		}
	}
}
