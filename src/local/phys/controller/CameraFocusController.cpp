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

#include "../../math/Matrix.hpp" // LookMatrix
#include "../../math/Vector.hpp"
#include "../Camera.hpp"
#include "../mixin/Controllable.hpp" // dynamic_cast
#include "../mixin/Positionable.hpp" // Positionable::GetPosition
#include "CameraFocusController.hpp"

namespace page
{
	namespace phys
	{
		// construct
		CameraFocusController::CameraFocusController(const Camera &camera, const Positionable &target) :
			Controller(constraintLayer), camera(camera), target(&target)
		{
			// initialize dependencies
			if (const Controllable *dependency = dynamic_cast<const Controllable *>(this->target))
				dependencies.push_back(dependency);
		}

		// clone
		CameraFocusController *CameraFocusController::Clone() const
		{
			return new CameraFocusController(*this);
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
			math::Vector<3> vector(target->GetPosition() - camera.GetPosition());
			frame.orientation = math::Quat<>(LookMatrix(Norm(vector)));
			frame.depth = Len(vector);
			return frame;
		}
	}
}
