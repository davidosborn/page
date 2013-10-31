#include <algorithm> // max, min

#include "../../cache/proxy/AabbProxy.hpp"
#include "../../inp/Driver.hpp" // Driver::Get{Lift,Rotation,Zoom}
#include "../../math/Aabb.hpp"
#include "../../math/Quat.hpp"
#include "../../math/Vector.hpp"
#include "../Form.hpp" // Form->Controllable
#include "HeroCamController.hpp"

namespace page { namespace phys
{
	// construct
	HeroCamController::HeroCamController(const inp::Driver &driver, const Form &target) :
		Controller(AnimationLayer::postConstraint), driver(driver), aabb(cache::AabbProxy(target, false)),
		orientation(target.GetOrientation()), center(.5), distance(4),
		dependencies(1, &target)
	{
		// FIXME: calculate min/max/default distance from AABB
	}

	// modifiers
	void HeroCamController::SetTarget(const Form &target)
	{
		aabb = cache::AabbProxy(target, false);
		dependencies.back() = &target;
	}

	// dependencies
	Controller::Dependencies HeroCamController::GetDependencies() const
	{
		return dependencies;
	}

	// generate frame
	void HeroCamController::DoUpdate(float deltaTime)
	{
		orientation += driver.GetRotation();
		distance = std::min(std::max(distance + driver.GetZoom(), 1.f), 8.f);
	}
	Frame HeroCamController::DoGetFrame(const Frame &, const Frame &) const
	{
		Frame frame;
		math::Quat<> orientation(this->orientation);
		frame.position = Center(*aabb, math::Vec3(.5, center, .5)) +
			orientation * (math::NormVector<3>() * distance);
		frame.orientation = orientation;
		return frame;
	}
}}
