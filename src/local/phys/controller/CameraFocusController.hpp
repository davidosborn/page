#ifndef    page_local_phys_controller_CameraFocusController_hpp
#   define page_local_phys_controller_CameraFocusController_hpp

#	include "../Controller.hpp"

namespace page { namespace phys
{
	class Camera;
	class Positionable;

	class CameraFocusController :
		public Controller,
		public virtual util::Cloneable<CameraFocusController, Controller>
	{
		public:
		// construct
		CameraFocusController(const Camera &, const Positionable &target);

		// modifiers
		void Track(const Positionable &target);

		// dependencies
		Dependencies GetDependencies() const;

		private:
		// generate frame
		Frame DoGetFrame(const Frame &, const Frame &) const;

		const Camera &camera;
		const Positionable *target;
		Dependencies dependencies;
	};
}}

#endif
