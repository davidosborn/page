#ifndef    page_local_phys_controller_IdleController_hpp
#   define page_local_phys_controller_IdleController_hpp

#	include "AnimationTargetController.hpp"

namespace page { namespace phys
{
	class IdleController :
		public AnimationTargetController,
		public virtual util::Cloneable<IdleController, AnimationTargetController>
	{
		public:
		// construct
		IdleController();

		private:
		// update hooks
		void UpdateTargets(float deltaTime);
	};
}}

#endif
