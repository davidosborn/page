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
		if (phys::BlinkController::Check(body))
			body.AttachController(phys::BlinkController(body));
		if (phys::EyeController::Check(body))
			body.AttachController(phys::EyeController(body));
		if (phys::LipsyncController::Check(body))
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
