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
	void Character::Goto(const math::Vec3 &target)
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
