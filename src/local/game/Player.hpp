#ifndef    page_local_game_Player_hpp
#   define page_local_game_Player_hpp

#	include <memory> // shared_ptr

namespace page { namespace inp { class Driver; }}

namespace page { namespace game
{
	class Character;

	struct Player
	{
		// construct/destroy
		explicit Player(const std::shared_ptr<Character> &);
		~Player();

		// character access
		std::shared_ptr<Character> GetCharacter();
		std::shared_ptr<const Character> GetCharacter() const;

		// update
		void Update(const inp::Driver &);

		private:
		std::shared_ptr<Character> character;
	};
}}

#endif
