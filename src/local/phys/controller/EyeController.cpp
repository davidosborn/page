#include <cassert>

#include "../attrib/Pose.hpp"
#include "../mixin/Controllable.hpp" // dynamic_cast
#include "../mixin/Positionable.hpp" // Positionable::GetPosition
#include "EyeController.hpp"

namespace page { namespace phys
{
	// construct
	EyeController::EyeController(const attrib::Pose &pose) :
		Controller(AnimationLayer::postConstraint), mode(noMode)
	{
		assert(Check(pose));
	}

	// modifiers
	void EyeController::Reset()
	{
		mode = noMode;
		// initialize dependencies
		dependencies.clear();
	}
	void EyeController::LookAt(const math::Vec3 &position)
	{
		lookAt.position = position;
		mode = lookAtMode;
		// initialize dependencies
		dependencies.clear();
	}
	void EyeController::Follow(const Positionable &target, const math::Vec3 &position)
	{
		follow.target = &target;
		follow.position = position;
		mode = followMode;
		// initialize dependencies
		dependencies.clear();
		if (const Controllable *dependency = dynamic_cast<const Controllable *>(follow.target))
			dependencies.push_back(dependency);
	}

	// dependencies
	Controller::Dependencies EyeController::GetDependencies() const
	{
		return dependencies;
	}

	// check compatibility
	bool EyeController::Check(const attrib::Pose &pose)
	{
		return
			pose.GetBone("eye.l") &&
			pose.GetBone("eye.r");
	}

	// generate frame
	Frame EyeController::DoGetFrame(const Frame &, const Frame &) const
	{
		Frame frame;
		// FIXME: implement; need to account for world position of head
		return frame;
	}
}}
