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

#include <algorithm> // max, min

#include "../../cache/proxy/AabbProxy.hpp"
#include "../../inp/Driver.hpp" // Driver::Get{Lift,Rotation,Zoom}
#include "../../math/Aabb.hpp"
#include "../../math/Quat.hpp"
#include "../../math/Vector.hpp"
#include "../node/Form.hpp" // Form->Node
#include "HeroCamController.hpp"

namespace page { namespace phys
{
	/*-------------+
	| constructors |
	+-------------*/

	HeroCamController::HeroCamController(const inp::Driver &driver, const Form &target) :
		Controller(AnimationLayer::postConstraint), driver(driver)
	{
		SetTarget(target);

		// FIXME: calculate min/max/default distance from AABB
	}

	/*----------+
	| modifiers |
	+----------*/

	void HeroCamController::SetTarget(const Form &target)
	{
		aabb = cache::AabbProxy(target, false);
		orientation = math::Euler<>(target.GetOrientation());
		center = .5;
		distance = 4;
		SetDependencies({&target});
	}

	/*--------------------------+
	| Controller implementation |
	+--------------------------*/

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
