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

#ifndef    page_local_phys_controller_FollowController_hpp
#   define page_local_phys_controller_FollowController_hpp

#	include "../../cache/proxy/Aabb.hpp"
#	include "../../math/Quat.hpp"
#	include "../../math/Vector.hpp"
#	include "../Controller.hpp"

namespace page { namespace phys
{
	class Form;

	class FollowController :
		public Controller,
		public virtual util::Cloneable<FollowController, Controller>
	{
		public:
		// construct
		explicit FollowController(
			const Form &target,
			const math::Vector<3> &center = .5,
			const math::Quat<> &orientation = math::Quat<>(),
			float distance = 0);

		// modifiers
		void Follow(const Form &target);
		void Follow(const Form &target, const math::Vector<3> &center);
		void Reorient(const math::Quat<> &orientation);
		void Reorient(const math::Quat<> &orientation, float distance);

		// dependencies
		Dependencies GetDependencies() const;

		private:
		// generate frame
		Frame DoGetFrame(const Frame &, const Frame &) const;

		cache::Aabb aabb;
		math::Vector<3> center;
		math::Quat<> orientation;
		float distance;
		Dependencies dependencies;
	};
}}

#endif
