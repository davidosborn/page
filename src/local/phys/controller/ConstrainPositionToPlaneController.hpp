#ifndef    page_local_phys_controller_ConstrainPositionToPlaneController_hpp
#   define page_local_phys_controller_ConstrainPositionToPlaneController_hpp

#	include "../../math/Plane.hpp"
#	include "Controller.hpp"

namespace page { namespace phys
{
	class ConstrainPositionToPlaneController : public Controller
	{
		IMPLEMENT_CLONEABLE(ConstrainPositionToPlaneController, Controller)

		public:
		// construct
		explicit ConstrainPositionToPlaneController(const math::Plane<3> &);

		// modifiers
		void SetPlane(const math::Plane<3> &);

		private:
		// generate frame
		Frame DoGetFrame(const Frame &, const Frame &) const;

		math::Plane<3> plane;
	};
}}

#endif
