/**
 * @section license
 *
 * Copyright (c) 2006-2013 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
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

#include "../phys/controller/AnimationController.hpp"
#include "../phys/controller/BlinkController.hpp"
#include "../phys/controller/EyeController.hpp"
#include "../phys/controller/GaitController.hpp"
#include "../phys/controller/LipsyncController.hpp"
#include "../phys/controller/PathfindingController.hpp"
#include "../res/type/Character.hpp"
#include "../util/lang.hpp" // EditSpeech
#include "Character.hpp"

// TEST: print
#include <iostream>

namespace page { namespace game
{
	// construct
	Character::Character(const res::Character &character) :
		Entity(character.name, character.model)
	{
		// initialize attributes
		if (character.radius) SetRadius(character.radius);
		SetScale(GetScale() * character.scale);
		// initialize controllers
		pathfindingController.reset(new phys::PathfindingController(*this));
		InsertController(pathfindingController);
		if (phys::BlinkController::Check(*this))
			InsertController(phys::BlinkController(*this));
		if (phys::EyeController::Check(*this))
			InsertController(phys::EyeController(*this));
		if (phys::LipsyncController::Check(*this))
			InsertController(phys::LipsyncController(*this));
		if (character.animation.ambient)
			InsertController(phys::AnimationController(*character.animation.ambient));
		if (character.gait)
			InsertController(phys::GaitController(*this, character.gait));
	}

	// actions
	void Character::Goto(const math::Vector<3> &target)
	{
		pathfindingController->Goto(target);
	}
	void Character::Say(const std::string &s)
	{
		// TEST: print
		std::cout << GetName() << ": " << util::EditSpeech(s) << std::endl;
	}

	// properties
	// FIXME: implement

	// conditions
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
