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

#include "../inp/Driver.hpp" // Driver::Get{Direction,Modifier}
#include "../math/Euler.hpp"
#include "../phys/controller/PathfindingController.hpp" // LocomotionController::SetForce, PathfindingController->LocomotionController
#include "../phys/node/Body.hpp"
#include "Character.hpp" // Character::{GetBody,pathfindingController}
#include "Player.hpp"

namespace page { namespace game
{
	/*-------------+
	| constructors |
	+-------------*/

	Player::Player(const res::Character &character) :
		Player(std::make_shared<Character>(character)) {}

	Player::Player(const std::shared_ptr<Character> &character) :
		character(character)
	{
		character->GetBody().SetType(phys::Collidable::Type::active);
	}

	Player::~Player()
	{
		character->GetBody().SetType(phys::Collidable::Type::passive);
	}

	/*-------+
	| update |
	+-------*/

	void Player::Update(const inp::Driver &driver)
	{
		math::Vec2 dir(driver.GetDirection() * (driver.GetModifier(inp::Modifier::run) + 1));
		math::Vec2 fwd(Rotate(math::Vec2(0, dir.y),
			math::Euler<>(character->GetBody().GetOrientation()).yaw));
		character->pathfindingController->SetForce(math::Vec3(fwd.x, 0, fwd.y) * 2);
		character->pathfindingController->SetForce(math::Euler<>(-dir.x, 0, 0) * 2);
	}

	/*-----------------+
	| character access |
	+-----------------*/

	std::shared_ptr<Character> Player::GetCharacter()
	{
		return character;
	}

	std::shared_ptr<const Character> Player::GetCharacter() const
	{
		return character;
	}
}}
