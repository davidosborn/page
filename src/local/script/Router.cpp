#include "../cache/proxy/Resource.hpp"
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
				new game::Character(*cache::Resource<res::Character>(path)));
			game->GetScene().Insert(character);
			return character;
		}
		std::shared_ptr<game::Object> Router::MakeObject(const std::string &path)
		{
			std::shared_ptr<game::Object> object(
				new game::Object(*cache::Resource<res::Object>(path)));
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
