#include "../../math/Matrix.hpp" // LookMatrix
#include "../../math/Vector.hpp"
#include "../Camera.hpp"
#include "../mixin/Controllable.hpp" // dynamic_cast
#include "../mixin/Positionable.hpp" // Positionable::GetPosition
#include "CameraFocusController.hpp"

namespace page { namespace phys
{
	// construct
	CameraFocusController::CameraFocusController(const Camera &camera, const Positionable &target) :
		Controller(AnimationLayer::postConstraint), camera(camera), target(&target)
	{
		// initialize dependencies
		if (const Controllable *dependency = dynamic_cast<const Controllable *>(this->target))
			dependencies.push_back(dependency);
	}

	// modifiers
	void CameraFocusController::Track(const Positionable &target)
	{
		this->target = &target;
		// initialize dependencies
		dependencies.clear();
		if (const Controllable *dependency = dynamic_cast<const Controllable *>(this->target))
			dependencies.push_back(dependency);
	}

	// dependencies
	Controller::Dependencies CameraFocusController::GetDependencies() const
	{
		return dependencies;
	}

	// generate frame
	Frame CameraFocusController::DoGetFrame(const Frame &, const Frame &) const
	{
		Frame frame;
		math::Vec3 vector(target->GetPosition() - camera.GetPosition());
		frame.orientation = math::Quat<>(LookMatrix(Norm(vector)));
		frame.depth = Len(vector);
		return frame;
	}
}}
