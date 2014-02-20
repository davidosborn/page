/**
 * @copyright
 *
 * Copyright (c) 2006-2014 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the same place
 *    and form as other copyright, license, and disclaimer information.
 *
 * 3. Redistributions in binary form must also include an acknowledgement in the
 *    same place and form as other acknowledgements (such as the credits),
 *    similar in substance to the following:
 *
 *       Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#include "../../cache/proxy/AabbProxy.hpp"
#include "../../math/Aabb.hpp"
#include "../node/Form.hpp" // Form->Node
#include "FollowController.hpp"

namespace page { namespace phys
{
	/*-------------+
	| constructors |
	+-------------*/

	FollowController::FollowController(const Form &target, const math::Vec3 &center, const math::Quat<> &orientation, float distance) :
		Controller(AnimationLayer::postConstraint)
	{
		Follow(target, center);
		Reorient(orientation, distance);
	}

	/*----------+
	| modifiers |
	+----------*/

	void FollowController::Follow(const Form &target)
	{
		aabb = cache::AabbProxy(target, false);
		SetDependencies({&target});
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

	/*--------------------------+
	| Controller implementation |
	+--------------------------*/

	Frame FollowController::DoGetFrame(const Frame &, const Frame &) const
	{
		Frame frame;
		frame.position = Center(*aabb, center) +
			orientation * math::NormVector<3>() * distance;
		frame.orientation = orientation;
		return frame;
	}
}}
