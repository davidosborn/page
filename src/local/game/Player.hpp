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
