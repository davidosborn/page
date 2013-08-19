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
