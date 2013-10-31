#ifndef    page_local_phys_controller_TargetController_hpp
#   define page_local_phys_controller_TargetController_hpp

#	include <vector>

#	include "Controller.hpp"

namespace page { namespace phys
{
	/**
	 * Interpolated frame matching controller.
	 *
	 * @note Similar attributes should be defined in each frame to avoid
	 * popping.
	 */
	class TargetController : public Controller
	{
		public:
		// construct
		explicit TargetController(AnimationLayer, float fadeDuration = 1);

		protected:
		// modifiers
		void SetTarget(const Frame &);
		void SetTarget(const Frame &, float fadeDuration);

		private:
		// update hooks
		virtual void UpdateTargets(float deltaTime) = 0;

		// update/generate frame
		void DoUpdate(float deltaTime);
		Frame DoGetFrame(const Frame &, const Frame &) const;

		float fadeDuration;
		struct Target
		{
			Frame frame;
			float opacity, fadeDuration;
		};
		typedef std::vector<Target> Targets;
		Targets targets;
	};
}}

#endif
