#include <cassert>
#include <cmath> // copysign

#include "../../math/Axan.hpp"
#include "../../math/float.hpp" // RadToDeg
#include "../attrib/PositionOrientation.hpp" // PositionOrientation::Get{Force,Normal}
#include "AnimationController.hpp"
#include "GaitController.hpp"

namespace page { namespace phys
{
	// construct
	GaitController::GaitController(const attrib::PositionOrientation &body, const cache::Proxy<res::Gait> &gait) :
		AnimationTargetController(postCollisionLayer, false), body(body),
		gait(gait.Lock()), state(noState), stride(0) {}

	// modifiers
	void GaitController::SetState(State state)
	{
		if (state == this->state) return;
		switch (state)
		{
			case noState:    SetTarget();           break;
			case runState:   SetState(gait->run);   break;
			case sneakState: SetState(gait->sneak); break;
			case turnState:  SetState(gait->turn);  break;
			case walkState:  SetState(gait->walk);  break;
			default: assert(!"invalid state");
		}
		this->state = state;
	}
	void GaitController::SetState(const res::Gait::State &state)
	{
		if (state.animation)
		{
			SetTarget(AnimationController(*state.animation));
			stride = state.stride * state.animation->duration;
		}
		else
		{
			SetTarget();
			stride = 0;
		}
	}

	// update hooks
	void GaitController::UpdateTargets(float deltaTime)
	{
		if (Any(body.GetForce()))
		{
			float force = Len(body.GetForce());
			// FIXME: switch to run state when force >= threshold
			SetState(walkState);
		}
		else if (body.GetTorque()) SetState(turnState);
		else SetState(noState);
	}
	float GaitController::FixDeltaTime(float deltaTime)
	{
		if (stride) switch (state)
		{
			case noState: break;
			case runState:
			case sneakState:
			case walkState:
			deltaTime = std::copysign(
				Len(body.GetForce()) / stride,
				Dot(body.GetNormal(), body.GetForce()));
			break;
			case turnState:
			{
				math::Axan<> torque(body.GetTorque());
				deltaTime = std::copysign(
					math::RadToDeg(torque.angle) / stride,
					Dot(torque.axis, math::UpVector<3>()));
			}
			break;
			default: assert(!"invalid state");

		}
		return deltaTime;
	}
}}
