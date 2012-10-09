#ifndef    page_game_InputDispatcher_hpp
#   define page_game_InputDispatcher_hpp

#	include "../util/NonCopyable.hpp"

namespace page
{
	namespace inp { class Driver; }

	namespace game
	{
		class Hud;
		class Menu;
		class Player;

		struct InputDispatcher : util::NonCopyable
		{
			InputDispatcher(inp::Driver &, Player &, Hud &, Menu &);
			~InputDispatcher();

			private:
			// signal handlers
			void OnKey(inp::key::Key);

			// signal connections
			util::ScopedConnection keyCon;

			Player &player;
			Hud &hud;
			Menu &menu;
		};
	}
}

#endif
