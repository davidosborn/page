#include "../../cache/proxy/AabbProxy.hpp"
#include "../../math/Aabb.hpp"
#include "../Form.hpp" // Form->Controllable
#include "FollowController.hpp"

namespace page { namespace phys
{
	// construct
	FollowController::FollowController(const Form &target, const math::Vec3 &center, const math::Quat<> &orientation, float distance) :
		Controller(constraintLayer), aabb(cache::AabbProxy(target, false)),
		center(center), orientation(orientation), distance(distance),
		dependencies(1, &target) {}

	// modifiers
	void FollowController::Follow(const Form &target)
	{
		aabb = cache::AabbProxy(target, false);
		dependencies.back() = &target;
	}
	void FollowController::Follow(const Form &target, const math::Vec3 &center)
	{
		Follow(target);
		this->center = center;
	}
	void FollowController::Reorient(const math::Quat<> &orientation)
	{
		this->orientation = orientation;
	}
	void FollowController::Reorient(const math::Quat<> &orientation, float distance)
	{
		Reorient(orientation);
		this->distance = distance;
	}

	// dependencies
	Controller::Dependencies FollowController::GetDependencies() const
	{
		return dependencies;
	}

	// generate frame
	Frame FollowController::DoGetFrame(const Frame &, const Frame &) const
	{
		Frame frame;
		frame.position = Center(*aabb, center) +
			orientation * math::NormVector<3>() * distance;
		frame.orientation = orientation;
		return frame;
	}
}}
