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

#ifndef    page_local_phys_controller_LocomotionController_hpp
#   define page_local_phys_controller_LocomotionController_hpp

#	include <boost/optional.hpp>

#	include "../../math/Euler.hpp"
#	include "../../math/Quat.hpp"
#	include "../../math/Vector.hpp"
#	include "Controller.hpp"

namespace page { namespace phys
{
	namespace attrib { class PositionOrientation; }

	/**
	 * Locomotion controller.
	 *
	 * Affects top-level position/orientation attributes.
	 * Supports either targeted movement or discrete velocity/rotation forces.
	 */
	class LocomotionController : public Controller
	{
		IMPLEMENT_CLONEABLE(LocomotionController, Controller)

		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit LocomotionController(const attrib::PositionOrientation &);

		/*-----------------+
		| target modifiers |
		+-----------------*/

		bool HasTarget() const;
		void SetTarget();
		void SetTarget(const math::Vec3 &position);
		void SetTarget(const math::Quat<> &orientation);
		void SetTarget(const math::Vec3 &position, const math::Quat<> &orientation);

		/*----------------+
		| force modifiers |
		+----------------*/

		void SetForce(const math::Vec3 &velocity);
		void SetForce(const math::Euler<> &rotation);
		void SetForce(const math::Vec3 &velocity, const math::Euler<> &rotation);

		/*-------------+
		| update hooks |
		+-------------*/

		protected:
		virtual void UpdateLocomotion();

		/*--------------------------+
		| Controller implementation |
		+--------------------------*/

		private:
		void DoUpdate(float deltaTime);
		Frame DoGetFrame(const Frame &, const Frame &) const;

		/*-------------+
		| data members |
		+-------------*/

		const attrib::PositionOrientation &controlled;

		// target
		boost::optional<math::Vec3> position;
		boost::optional<math::Quat<>> orientation;

		// force
		math::Vec3 velocity, translation;
		math::Euler<> angularVelocity, rotation;
	};
}}

#endif
