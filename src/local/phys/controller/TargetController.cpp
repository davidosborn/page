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

#include "TargetController.hpp"

namespace page
{
	namespace phys
	{
		// construct
		TargetController::TargetController(Layer layer, float fadeDuration) :
			Controller(layer), fadeDuration(fadeDuration) {}

		// modifiers
		void TargetController::SetTarget(const Frame &frame)
		{
			SetTarget(frame, fadeDuration);
		}
		void TargetController::SetTarget(const Frame &frame, float fadeDuration)
		{
			Target target =
			{
				frame, 0, fadeDuration
			};
			targets.push_back(target);
		}

		// update/generate frame
		void TargetController::DoUpdate(float deltaTime)
		{
			UpdateTargets(deltaTime);
			for (Targets::reverse_iterator target(targets.rbegin()); target != targets.rend(); ++target)
			{
				if (target->opacity == 1) break;
				if ((target->opacity += deltaTime / target->fadeDuration) >= 1)
				{
					target->opacity = 1;
					targets.erase(targets.begin(), target.base());
					break;
				}
			}
		}
		Frame TargetController::DoGetFrame(const Frame &baseFrame, const Frame &accumFrame) const
		{
			Frame frame;
			if (!targets.empty())
			{
				frame = accumFrame;
				for (Targets::const_iterator target(targets.begin()); target != targets.end(); ++target)
					Blend(frame, target->frame, target->opacity, baseFrame);
			}
			return frame;
		}
	}
}
