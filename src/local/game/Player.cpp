#include "../inp/Driver.hpp" // Driver::Get{Direction,Modifier}
#include "../math/Euler.hpp"
#include "../math/Vector.hpp"
#include "../phys/controller/PathfindingController.hpp" // LocomotionController::SetForce, PathfindingController->LocomotionController
#include "Character.hpp" // Character::pathfindingController
#include "Player.hpp"

namespace page { namespace game
{
	// construct/destroy
	Player::Player(const std::shared_ptr<Character> &character) :
		character(character)
	{
		character->SetType(phys::Collidable::active);
	}
	Player::~Player()
	{
		character->SetType(phys::Collidable::passive);
	}

	// character access
	std::shared_ptr<Character> Player::GetCharacter()
	{
		return character;
	}
	std::shared_ptr<const Character> Player::GetCharacter() const
	{
		return character;
	}

	// update
	void Player::Update(const inp::Driver &driver)
	{
		math::Vec2 dir(driver.GetDirection() * (driver.GetModifier(inp::Driver::runModifier) + 1));
		math::Vec2 fwd(Rotate(math::Vec2(0, dir.y),
			math::Euler<>(character->GetOrientation()).yaw));
		character->pathfindingController->SetForce(math::Vec3(fwd.x, 0, fwd.y) * 2);
		character->pathfindingController->SetForce(math::Euler<>(-dir.x, 0, 0) * 2);
	}
}}
