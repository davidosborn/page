// constant frame controller
// can provide a persistent bind frame for other controllers to build on

#ifndef    page_local_phys_controller_FrameController_hpp
#   define page_local_phys_controller_FrameController_hpp

#	include "../Controller.hpp"

namespace page { namespace phys
{
	class FrameController :
		public Controller,
		public virtual util::Cloneable<FrameController, Controller>
	{
		public:
		// construct
		explicit FrameController(const Frame &, Layer = preCollisionLayer);

		private:
		// generate frame
		Frame DoGetFrame(const Frame &, const Frame &) const;

		Frame frame;
	};
}}

#endif
