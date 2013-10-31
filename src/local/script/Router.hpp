#ifndef    page_local_script_Router_hpp
#   define page_local_script_Router_hpp

#	include <memory> // shared_ptr
#	include <string>

namespace page { namespace game
{
	class Character;
	class Game;
	class Object;
}}

namespace page { namespace script
{
	struct Router
	{
		// construct
		explicit Router(game::Game &);

		// operations
		void Quit();

		// entity management
		std::shared_ptr<game::Character> MakeCharacter(const std::string &);
		std::shared_ptr<game::Object> MakeObject(const std::string &);
		void Remove(const std::shared_ptr<game::Character> &);
		void Remove(const std::shared_ptr<game::Object> &);

		private:
		game::Game *game;
	};
}}

#endif
