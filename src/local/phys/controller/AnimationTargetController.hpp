// interpolated animation switching controller
// similar attributes should be controlled in each animation to avoid popping
// animations should have same duration if synchronizing

#ifndef    page_local_phys_controller_AnimationTargetController_hpp
#   define page_local_phys_controller_AnimationTargetController_hpp

#	include <memory> // shared_ptr
#	include <vector>

#	include "../Controller.hpp"

namespace page { namespace phys
{
	class AnimationController;

	class AnimationTargetController : public Controller
	{
		public:
		// construct
		explicit AnimationTargetController(Layer, bool sync = true);

		// modifiers
		void SetTarget(); // no target
		void SetTarget(const AnimationController &);
		void SetTarget(const std::shared_ptr<AnimationController> &);

		private:
		// update hooks
		virtual void UpdateTargets(float deltaTime) = 0;
		virtual float FixDeltaTime(float deltaTime);

		// update/generate frame
		void DoUpdate(float deltaTime);
		Frame DoGetFrame(const Frame &, const Frame &) const;

		bool sync;
		float playPosition;
		typedef std::vector<std::shared_ptr<AnimationController>> Targets;
		Targets targets;
	};
}}

#endif
