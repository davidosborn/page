#ifndef    page_local_game_Character_hpp
#   define page_local_game_Character_hpp

#	include <memory> // shared_ptr

#	include "Entity.hpp"

namespace page
{
	namespace phys { class PathfindingController; }
	namespace res { class Character; }
}

namespace page { namespace game
{
	class Character : public Entity
	{
		friend class Player;

		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit Character(const res::Character &);

		/*--------+
		| actions |
		+--------*/

		void Goto(const math::Vec3 &);
		void Say(const std::string &);

		/*----------+
		| observers |
		+----------*/

		bool IsIdle() const;
		bool IsMoving() const;

		/*-------------+
		| data members |
		+-------------*/

		private:
		std::shared_ptr<phys::PathfindingController> pathfindingController;
	};
}}

#endif
