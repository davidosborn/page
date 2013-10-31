#ifndef    page_local_phys_controller_FrameController_hpp
#   define page_local_phys_controller_FrameController_hpp

#	include "Controller.hpp"

namespace page { namespace phys
{
	/**
	 * Constant frame controller.  Provides a persistent bind frame that other
	 * controllers can build on.
	 */
	class FrameController : public Controller
	{
		IMPLEMENT_CLONEABLE(FrameController, Controller)

		public:
		// construct
		explicit FrameController(const Frame &, AnimationLayer = AnimationLayer::preCollision);

		private:
		// generate frame
		Frame DoGetFrame(const Frame &, const Frame &) const;

		Frame frame;
	};
}}

#endif
