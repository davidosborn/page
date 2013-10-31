#include <cassert>

#include "../../math/Axan.hpp"
#include "../../math/float.hpp" // DegToRad
#include "../../math/interp.hpp" // LinearInterp
#include "../attrib/Pose.hpp"
#include "BlinkController.hpp"

namespace page { namespace phys
{
	const float
		openAngle = math::DegToRad(40.f),
		blinkRate = .25f,
		blinkSpeed = 30.f,
		blinkDelay = .05f;

	// construct
	BlinkController::BlinkController(const attrib::Pose &pose) :
		Controller(AnimationLayer::preCollision), state(openState), open(1.f)
	{
		assert(Check(pose));
		// FIXME: we should use bind-pose as open (relaxed) orientation and
		// calculated closed orientation using angle from upper eyelid to
		// lower eyelid, lower eyelid being a dummy bone or attachment point
		// bind-pose (closed orientation)
		const attrib::Pose::Bone *leftEyelidBone(pose.GetBone("eyelid.l"));
		if (leftEyelidBone) leftEyelidBindOrientation = leftEyelidBone->GetBindOrientation();
		const attrib::Pose::Bone *rightEyelidBone(pose.GetBone("eyelid.r"));
		if (rightEyelidBone) rightEyelidBindOrientation = rightEyelidBone->GetBindOrientation();
		// open orientation
		math::Vec3
			leftEyelidHinge(Norm(Cross(leftEyelidBindOrientation * math::NormVector<3>(), math::UpVector<3>()))),
			rightEyelidHinge(Norm(Cross(rightEyelidBindOrientation * math::NormVector<3>(), math::UpVector<3>())));
		leftEyelidOpenOrientation = leftEyelidBindOrientation * math::Axan<>(leftEyelidHinge, openAngle);
		rightEyelidOpenOrientation = rightEyelidBindOrientation * math::Axan<>(rightEyelidHinge, openAngle);
	}

	// check compatibility
	bool BlinkController::Check(const attrib::Pose &pose)
	{
		return
			pose.GetBone("eyelid.l") &&
			pose.GetBone("eyelid.r") &&
			pose.GetBone("lowereyelid.l") &&
			pose.GetBone("lowereyelid.r");
	}

	// update/generate frame
	void BlinkController::DoUpdate(float deltaTime)
	{
		switch (state)
		{
			case openState:
			if (!(rand() % (int(1.f / (blinkRate * deltaTime)) + 1)))
				state = closingState;
			break;
			case closingState:
			if ((open -= blinkSpeed * deltaTime) <= 0.f)
			{
				open = 0.f;
				state = closedState;
				delay = 0.f;
			}
			break;
			case closedState:
			if ((delay += deltaTime) >= blinkDelay)
				state = openingState;
			break;
			case openingState:
			if ((open += blinkSpeed * deltaTime) >= 1.f)
			{
				open = 1.f;
				state = openState;
			}
			break;
		}
	}
	Frame BlinkController::DoGetFrame(const Frame &, const Frame &) const
	{
		Frame frame;
		frame.bones["eyelid.l"].orientation = LinearInterp(leftEyelidBindOrientation, leftEyelidOpenOrientation, open);
		frame.bones["eyelid.r"].orientation = LinearInterp(rightEyelidBindOrientation, rightEyelidOpenOrientation, open);
		return frame;
	}
}}
