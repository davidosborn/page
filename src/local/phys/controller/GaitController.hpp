#ifndef    page_local_phys_controller_GaitController_hpp
#   define page_local_phys_controller_GaitController_hpp

#	include "../../cache/proxy/Proxy.hpp" // Proxy::Instance
#	include "../../res/type/Gait.hpp" // Gait::State
#	include "AnimationTargetController.hpp"

namespace page { namespace phys
{
	namespace attrib { class PositionOrientation; }

	class GaitController :
		public AnimationTargetController,
		public virtual util::Cloneable<GaitController, AnimationTargetController>
	{
		public:
		// construct
		GaitController(
			const attrib::PositionOrientation &,
			const cache::Proxy<res::Gait> &);

		private:
		enum State
		{
			noState,
			runState,
			sneakState,
			turnState,
			walkState
		};

		// modifiers
		void SetState(State);
		void SetState(const res::Gait::State &);

		// update hooks
		void UpdateTargets(float deltaTime);
		float FixDeltaTime(float deltaTime);

		const attrib::PositionOrientation &body;
		cache::Proxy<res::Gait>::Instance gait;
		State state;
		float stride;
	};
}}

#endif
