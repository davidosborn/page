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

#ifndef    page_local_phys_controller_BlinkController_hpp
#   define page_local_phys_controller_BlinkController_hpp

#	include <random> // mt19937

#	include "../../math/Quat.hpp"
#	include "Controller.hpp"

namespace page { namespace phys
{
	namespace attrib { class Pose; }

	/**
	 * Controls the natural blinking of a character's eyes.
	 *
	 * @note The controller expects the character to have their eyes closed in
	 * their bind pose.
	 */
	class BlinkController : public Controller
	{
		IMPLEMENT_CLONEABLE(BlinkController, Controller)

		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit BlinkController(const attrib::Pose &);

		/**
		 * Returns @c true if the given pose has the necessary bones to work
		 * with this controller.
		 */
		static bool IsCompatibleWith(const attrib::Pose &);

		/*--------------------------+
		| Controller implementation |
		+--------------------------*/

		private:
		void DoUpdate(float deltaTime);
		Frame DoGetFrame(const Frame &, const Frame &) const;

		/*-------------+
		| data members |
		+-------------*/

		enum State
		{
			openState,
			closingState,
			closedState,
			openingState
		} state;
		float open, delay;
		math::Quat<>
			leftEyelidBindOrientation, rightEyelidBindOrientation,
			leftEyelidOpenOrientation, rightEyelidOpenOrientation;
		std::mt19937 rand;
	};
}}

#endif
