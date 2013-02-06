/**
 * @section license
 *
 * Copyright (c) 2006-2012 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.

 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution, and in the same
 *    place and form as other copyright, license, and disclaimer information.
 *
 * As a special exception, distributions of derivative works in binary form may
 * include an acknowledgement in place of the above copyright notice, this list
 * of conditions, and the following disclaimer in the documentation and/or other
 * materials provided with the distribution, and in the same place and form as
 * other acknowledgements, similar in substance to the following:
 *
 *    Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#include "../inp/Driver.hpp" // Driver::Get{Direction,Modifier}
#include "../math/Euler.hpp"
#include "../math/Vector.hpp"
#include "../phys/controller/PathfindingController.hpp" // LocomotionController::SetForce, PathfindingController->LocomotionController
#include "Character.hpp" // Character::pathfindingController
#include "Player.hpp"

namespace page
{
	namespace game
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
			math::Vector<2> dir(driver.GetDirection() * (driver.GetModifier(inp::Driver::runModifier) + 1));
			math::Vector<2> fwd(Rotate(math::Vector<2>(0, dir.y),
				math::Euler<>(character->GetOrientation()).yaw));
			character->pathfindingController->SetForce(math::Vector<3>(fwd.x, 0, fwd.y) * 2);
			character->pathfindingController->SetForce(math::Euler<>(-dir.x, 0, 0) * 2);
		}
	}
}
