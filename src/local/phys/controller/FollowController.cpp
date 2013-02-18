/**
 * @section license
 *
 * Copyright (c) 2006-2012 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution, and in the same
 *    place and form as other copyright, license, and disclaimer information.
 *
 * As a special exception, distributions of derivative works in binary form may
 * include an acknowledgement in place of the above copyright notice, this list
 * of conditions, and the following disclaimer in the documentation and/or other
 * materials provided with the distribution, and in the same place and form as
 * other acknowledgements, similar in substance to the following:
 *
 *    Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#include "../../math/Aabb.hpp"
#include "../Form.hpp" // Form->Controllable
#include "FollowController.hpp"

namespace page
{
	namespace phys
	{
		// construct
		FollowController::FollowController(const Form &target, const math::Vector<3> &center, const math::Quat<> &orientation, float distance) :
			Controller(constraintLayer), aabb(target, false), center(center),
			orientation(orientation), distance(distance),
			dependencies(1, &target) {}

		// clone
		FollowController *FollowController::Clone() const
		{
			return new FollowController(*this);
		}

		// modifiers
		void FollowController::Follow(const Form &target)
		{
			aabb = cache::Aabb(target, false);
			dependencies.back() = &target;
		}
		void FollowController::Follow(const Form &target, const math::Vector<3> &center)
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
			frame.position = Center(*aabb, center)
				orientation * math::NormVector<3>() * distance;
			frame.orientation = orientation;
			return frame;
		}
	}
}
