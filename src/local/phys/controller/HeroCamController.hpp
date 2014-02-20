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

#ifndef    page_local_phys_controller_HeroCamController_hpp
#   define page_local_phys_controller_HeroCamController_hpp

#	include "../../cache/proxy/Proxy.hpp"
#	include "../../math/Euler.hpp"
#	include "../../math/fwd.hpp" // Aabb
#	include "Controller.hpp"

namespace page { namespace inp { class Driver; }}

namespace page { namespace phys
{
	class Form;

	class HeroCamController : public Controller
	{
		IMPLEMENT_CLONEABLE(HeroCamController, Controller)

		/*-------------+
		| constructors |
		+-------------*/

		public:
		HeroCamController(const inp::Driver &, const Form &);

		/*----------+
		| modifiers |
		+----------*/

		void SetTarget(const Form &);

		/*--------------------------+
		| Controller implementation |
		+--------------------------*/

		private:
		void DoUpdate(float deltaTime);
		Frame DoGetFrame(const Frame &, const Frame &) const;

		/*-------------+
		| data members |
		+-------------*/

		const inp::Driver &driver;
		cache::Proxy<math::Aabb<3>> aabb;
		math::Euler<> orientation;
		float center, distance;
	};
}}

#endif
