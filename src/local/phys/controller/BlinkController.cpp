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

	/*-------------+
	| constructors |
	+-------------*/

	BlinkController::BlinkController(const attrib::Pose &pose) :
		Controller(AnimationLayer::preCollision), state(openState), open(1.f)
	{
		assert(IsCompatibleWith(pose));
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

	bool BlinkController::IsCompatibleWith(const attrib::Pose &pose)
	{
		return
			pose.GetBone("eyelid.l") &&
			pose.GetBone("eyelid.r") &&
			pose.GetBone("lowereyelid.l") &&
			pose.GetBone("lowereyelid.r");
	}

	/*--------------------------+
	| Controller implementation |
	+--------------------------*/

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
