#ifndef    page_local_game_InputDispatcher_hpp
#   define page_local_game_InputDispatcher_hpp

#	include <boost/signals/connection.hpp>

#	include "../util/class/special_member_functions.hpp" // Uncopyable

namespace page { namespace inp { class Driver; }}

namespace page { namespace game
{
	class Hud;
	class Menu;
	class Player;

	struct InputDispatcher : util::Uncopyable<InputDispatcher>
	{
		InputDispatcher(inp::Driver &, Player &, Hud &, Menu &);
		~InputDispatcher();

		private:
		// signal handlers
		void OnKey(inp::key::Key);

		// signal connections
		boost::signals::scoped_connection keyCon;

		Player &player;
		Hud &hud;
		Menu &menu;
	};
}}

#endif
