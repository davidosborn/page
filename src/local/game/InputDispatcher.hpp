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
