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

#include <cassert>

#include "../attrib/Pose.hpp"
#include "../mixin/Controllable.hpp" // dynamic_cast
#include "../mixin/Positionable.hpp" // Positionable::GetPosition
#include "EyeController.hpp"

namespace page
{
	namespace phys
	{
		// construct
		EyeController::EyeController(const attrib::Pose &pose) :
			Controller(constraintLayer), mode(noMode)
		{
			assert(Check(pose));
		}

		// clone
		EyeController *EyeController::Clone() const
		{
			return new EyeController(*this);
		}

		// modifiers
		void EyeController::Reset()
		{
			mode = noMode;
			// initialize dependencies
			dependencies.clear();
		}
		void EyeController::LookAt(const math::Vector<3> &position)
		{
			lookAt.position = position;
			mode = lookAtMode;
			// initialize dependencies
			dependencies.clear();
		}
		void EyeController::Follow(const Positionable &target, const math::Vector<3> &position)
		{
			follow.target = &target;
			follow.position = position;
			mode = followMode;
			// initialize dependencies
			dependencies.clear();
			if (const Controllable *dependency = dynamic_cast<const Controllable *>(follow.target))
				dependencies.push_back(dependency);
		}

		// dependencies
		Controller::Dependencies EyeController::GetDependencies() const
		{
			return dependencies;
		}

		// check compatibility
		bool EyeController::Check(const attrib::Pose &pose)
		{
			return
				pose.GetBone("eye.l") &&
				pose.GetBone("eye.r");
		}

		// generate frame
		Frame EyeController::DoGetFrame(const Frame &, const Frame &) const
		{
			Frame frame;
			// FIXME: implement; need to account for world position of head
			return frame;
		}
	}
}
