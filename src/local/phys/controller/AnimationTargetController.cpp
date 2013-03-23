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

#include "AnimationController.hpp" // AnimationController::{~AnimationController,Get{Frame,Opacity},Update}
#include "AnimationTargetController.hpp"

namespace page
{
	namespace phys
	{
		// construct
		AnimationTargetController::AnimationTargetController(Layer layer, bool sync) :
			Controller(layer), sync(sync), playPosition(0) {}

		// modifiers
		void AnimationTargetController::SetTarget()
		{
			for (Targets::iterator iter(targets.begin()); iter != targets.end(); ++iter)
			{
				AnimationController &target(**iter);
				target.Kill();
			}
		}
		void AnimationTargetController::SetTarget(const AnimationController &target)
		{
			SetTarget(std::shared_ptr<AnimationController>(target.Clone()));
		}
		void AnimationTargetController::SetTarget(const std::shared_ptr<AnimationController> &target)
		{
			if (sync) target->SetPlayPosition(playPosition);
			targets.push_back(target);
		}

		// update hooks
		float AnimationTargetController::FixDeltaTime(float deltaTime)
		{
			return deltaTime;
		}

		// update/generate frame
		void AnimationTargetController::DoUpdate(float deltaTime)
		{
			UpdateTargets(deltaTime);
			float fixedDeltaTime = FixDeltaTime(deltaTime);
			playPosition += fixedDeltaTime;
			if (!targets.empty())
			{
				// update targets
				for (Targets::iterator iter(targets.begin()); iter != targets.end();)
				{
					AnimationController &target(**iter);
					target.Update(deltaTime, fixedDeltaTime);
					if (!target.IsAlive())
					{
						iter = targets.erase(iter);
						continue;
					}
					++iter;
				}
				// remove occluded targets
				if (!targets.empty())
				{
					for (Targets::reverse_iterator iter(targets.rbegin()); iter != targets.rend() - 1; ++iter)
					{
						AnimationController &target(**iter);
						if (target.GetOpacity() == 1)
						{
							targets.erase(targets.begin(), iter.base());
							break;
						}
					}
				}
			}
		}
		Frame AnimationTargetController::DoGetFrame(const Frame &baseFrame, const Frame &accumFrame) const
		{
			Frame frame;
			if (!targets.empty())
			{
				frame = baseFrame;
				for (Targets::const_iterator iter(targets.begin()); iter != targets.end(); ++iter)
				{
					const AnimationController &target(**iter);
					Blend(frame, target.GetFrame(baseFrame, frame), target.GetOpacity(), baseFrame);
				}
			}
			return frame;
		}
	}
}
