#include "AnimationController.hpp"
#include "IdleController.hpp"

namespace page { namespace phys
{
	// construct
	IdleController::IdleController() :
		AnimationTargetController(AnimationLayer::preCollision) {}

	// update hooks
	void IdleController::UpdateTargets(float deltaTime)
	{
		// FIXME: implement
	}
}}
