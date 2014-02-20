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

#ifndef    page_local_game_Player_hpp
#   define page_local_game_Player_hpp

#	include <memory> // shared_ptr

#	include "../util/class/special_member_functions.hpp" // Polymorphic

namespace page
{
	namespace inp { class Driver; }
	namespace res { class Character; }
}

namespace page { namespace game
{
	class Character;

	class Player : public util::Polymorphic<Player>
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit Player(const res::Character &);
		explicit Player(const std::shared_ptr<Character> &);
		~Player() override;

		/*-------+
		| update |
		+-------*/

		void Update(const inp::Driver &);

		/*-----------------+
		| character access |
		+-----------------*/

		std::shared_ptr<Character> GetCharacter();
		std::shared_ptr<const Character> GetCharacter() const;

		/*-------------+
		| data members |
		+-------------*/

		private:
		std::shared_ptr<Character> character;
	};
}}

#endif
