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

#include "../phys/controller/AnimationController.hpp"
#include "../phys/controller/BlinkController.hpp"
#include "../phys/controller/EyeController.hpp"
#include "../phys/controller/GaitController.hpp"
#include "../phys/controller/LipsyncController.hpp"
#include "../phys/controller/PathfindingController.hpp"
#include "../phys/node/Body.hpp"
#include "../res/type/Character.hpp"
#include "../util/lang.hpp" // EditSpeech
#include "Character.hpp"

// TEST: print
#include <iostream>

namespace page { namespace game
{
	/*-------------+
	| constructors |
	+-------------*/

	Character::Character(const res::Character &character) :
		Entity(character.name, character.model)
	{
		auto &body(GetBody());

		// initialize attributes
		if (character.radius) body.SetRadius(character.radius);
		body.SetScale(body.GetScale() * character.scale);

		// initialize controllers
		pathfindingController.reset(new phys::PathfindingController(body));
		body.AttachController(pathfindingController);
		if (phys::BlinkController::IsCompatibleWith(body))
			body.AttachController(phys::BlinkController(body));
		if (phys::EyeController::IsCompatibleWith(body))
			body.AttachController(phys::EyeController(body));
		if (phys::LipsyncController::IsCompatibleWith(body))
			body.AttachController(phys::LipsyncController(body));
		if (character.animation.ambient)
			body.AttachController(phys::AnimationController(*character.animation.ambient));
		if (character.gait)
			body.AttachController(phys::GaitController(body, character.gait));
	}

	/*--------+
	| actions |
	+--------*/

	void Character::Goto(const math::Vec3 &target)
	{
		pathfindingController->Goto(target);
	}

	void Character::Say(const std::string &s)
	{
		// TEST: print
		std::cout << GetName() << ": " << util::EditSpeech(s) << std::endl;
	}

	/*----------+
	| observers |
	+----------*/

	bool Character::IsIdle() const
	{
		// FIXME: implement
		return true;
	}

	bool Character::IsMoving() const
	{
		// FIXME: implement
		return false;
	}
}}
