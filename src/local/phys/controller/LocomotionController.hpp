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

// locomotion controller
// affects top-level position/orientation attributes
// supports either targeted movement or discrete velocity/rotation forces

#ifndef    page_local_phys_controller_LocomotionController_hpp
#   define page_local_phys_controller_LocomotionController_hpp

#	include "../../math/Euler.hpp"
#	include "../../math/Quat.hpp"
#	include "../../math/Vector.hpp"
#	include "../../util/Optional.hpp"
#	include "../Controller.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib { class PositionOrientation; }

		struct LocomotionController : Controller
		{
			// construct
			explicit LocomotionController(const attrib::PositionOrientation &);

			// clone
			LocomotionController *Clone() const;

			// target modifiers
			bool HasTarget() const;
			void SetTarget();
			void SetTarget(const math::Vector<3> &position);
			void SetTarget(const math::Quat<> &orientation);
			void SetTarget(const math::Vector<3> &position, const math::Quat<> &orientation);

			// force modifiers
			void SetForce(const math::Vector<3> &velocity);
			void SetForce(const math::Euler<> &rotation);
			void SetForce(const math::Vector<3> &velocity, const math::Euler<> &rotation);

			protected:
			// update hooks
			virtual void UpdateLocomotion();

			private:
			// update/generate frame
			void DoUpdate(float deltaTime);
			Frame DoGetFrame(const Frame &, const Frame &) const;

			const attrib::PositionOrientation &controlled;

			// target
			util::Optional<math::Vector<3>> position;
			util::Optional<math::Quat<>> orientation;

			// force
			math::Vector<3> velocity, translation;
			math::Euler<> angularVelocity, rotation;
		};
	}
}

#endif
