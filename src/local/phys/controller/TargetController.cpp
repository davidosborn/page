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
