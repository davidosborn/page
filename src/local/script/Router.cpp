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

#include "../cache/proxy/ResourceProxy.hpp"
#include "../game/Character.hpp"
#include "../game/Game.hpp" // Game::{GetScene,Quit}
#include "../game/Object.hpp"
#include "../game/Scene.hpp" // Scene::Insert
#include "Router.hpp"

namespace page
{
	namespace script
	{
		// construct
		Router::Router(game::Game &game) : game(&game) {}

		// operations
		void Router::Quit()
		{
			game->Quit();
		}

		// entity management
		std::shared_ptr<game::Character> Router::MakeCharacter(const std::string &path)
		{
			std::shared_ptr<game::Character> character(
				new game::Character(*cache::ResourceProxy<res::Character>(path)));
			game->GetScene().Insert(character);
			return character;
		}
		std::shared_ptr<game::Object> Router::MakeObject(const std::string &path)
		{
			std::shared_ptr<game::Object> object(
				new game::Object(*cache::ResourceProxy<res::Object>(path)));
			game->GetScene().Insert(object);
			return object;
		}
		void Router::Remove(const std::shared_ptr<game::Character> &character)
		{
			game->GetScene().Remove(character);
		}
		void Router::Remove(const std::shared_ptr<game::Object> &object)
		{
			game->GetScene().Remove(object);
		}
	}
}
