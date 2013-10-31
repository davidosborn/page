#include "../attrib/PositionOrientation.hpp" // PositionOrientation::Get{Orientation,Position}
#include "LocomotionController.hpp"

namespace page { namespace phys
{
	// construct
	LocomotionController::LocomotionController(const attrib::PositionOrientation &controlled) :
		Controller(AnimationLayer::preCollision), controlled(controlled) {}

	// target modifiers
	bool LocomotionController::HasTarget() const
	{
		return position || orientation;
	}
	void LocomotionController::SetTarget()
	{
		position.reset();
		orientation.reset();
	}
	void LocomotionController::SetTarget(const math::Vec3 &position)
	{
		this->position = position;
		velocity = 0;
	}
	void LocomotionController::SetTarget(const math::Quat<> &orientation)
	{
		this->orientation = orientation;
		angularVelocity = 0;
	}
	void LocomotionController::SetTarget(const math::Vec3 &position, const math::Quat<> &orientation)
	{
		this->position    = position;
		this->orientation = orientation;
		velocity        = 0;
		angularVelocity = 0;
	}

	// state modifiers
	void LocomotionController::SetForce(const math::Vec3 &velocity)
	{
		this->velocity = velocity;
		position.reset();
	}
	void LocomotionController::SetForce(const math::Euler<> &angularVelocity)
	{
		this->angularVelocity = angularVelocity;
		orientation.reset();
	}
	void LocomotionController::SetForce(const math::Vec3 &velocity, const math::Euler<> &angularVelocity)
	{
		this->velocity        = velocity;
		this->angularVelocity = angularVelocity;
		SetTarget();
	}

	// update hooks
	void LocomotionController::UpdateLocomotion() {}

	// update/generate frame
	void LocomotionController::DoUpdate(float deltaTime)
	{
		UpdateLocomotion();
		// FIXME: clear each target after arriving at it
		// FIXME: consider supporting 2D position targets for characters
		// FIXME: determine the velocity for the target; it should be
		// specified when setting the target or on initialization
		if (position)
		{
			// FIXME: implement
			translation = *position - controlled.GetPosition();
		}
		else translation = velocity * deltaTime;
		if (orientation)
		{
			// FIXME: implement
		}
		else rotation = angularVelocity * deltaTime;
	}
	Frame LocomotionController::DoGetFrame(const Frame &baseFrame, const Frame &) const
	{
		Frame frame;
		frame.position = *baseFrame.position + translation;
		frame.orientation = *baseFrame.orientation * rotation;
		return frame;
	}
}}
