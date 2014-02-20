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

#include <cassert>

#include "../attrib/Pose.hpp"
#include "../mixin/Positionable.hpp" // Positionable::GetPosition
#include "../node/Node.hpp" // dynamic_cast
#include "EyeController.hpp"

namespace page { namespace phys
{
	/*-------------+
	| constructors |
	+-------------*/

	EyeController::EyeController(const attrib::Pose &pose) :
		Controller(AnimationLayer::postConstraint), mode(noMode)
	{
		assert(IsCompatibleWith(pose));
	}

	bool EyeController::IsCompatibleWith(const attrib::Pose &pose)
	{
		return
			pose.GetBone("eye.l") &&
			pose.GetBone("eye.r");
	}

	/*----------+
	| modifiers |
	+----------*/

	void EyeController::Reset()
	{
		mode = noMode;
		SetDependencies({});
	}

	void EyeController::LookAt(const math::Vec3 &position)
	{
		lookAt.position = position;
		mode = lookAtMode;
		SetDependencies({});
	}

	void EyeController::Follow(const Positionable &target, const math::Vec3 &position)
	{
		follow.target = &target;
		follow.position = position;
		mode = followMode;

		// set dependencies
		Dependencies dependencies;
		if (auto dependency = dynamic_cast<const Node *>(follow.target))
			dependencies.push_back(dependency);
		SetDependencies(dependencies);
	}

	/*--------------------------+
	| Controller implementation |
	+--------------------------*/

	Frame EyeController::DoGetFrame(const Frame &, const Frame &) const
	{
		Frame frame;
		// FIXME: implement; need to account for world position of head
		return frame;
	}
}}
