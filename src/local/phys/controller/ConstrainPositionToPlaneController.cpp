#include "../../math/intersect.hpp" // ClosestPointOnPlane
#include "ConstrainPositionToPlaneController.hpp"

namespace page { namespace phys
{
	// construct
	ConstrainPositionToPlaneController::ConstrainPositionToPlaneController(const math::Plane<3> &plane) :
		Controller(constraintLayer), plane(plane) {}

	// modifiers
	void ConstrainPositionToPlaneController::SetPlane(const math::Plane<3> &plane)
	{
		this->plane = plane;
	}

	// generate frame
	Frame ConstrainPositionToPlaneController::DoGetFrame(const Frame &, const Frame &accum) const
	{
		Frame frame;
		if (accum.position)
			frame.position = ClosestPointOnPlane(plane, *accum.position);
		return frame;
	}
}}
