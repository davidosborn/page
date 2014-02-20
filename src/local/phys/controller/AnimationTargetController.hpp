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

#ifndef    page_local_phys_controller_AnimationTargetController_hpp
#   define page_local_phys_controller_AnimationTargetController_hpp

#	include <memory> // shared_ptr
#	include <vector>

#	include "Controller.hpp"

namespace page { namespace phys
{
	class AnimationController;

	/**
	 * An interpolated animation-switching controller.
	 *
	 * @note Similar attributes should be controlled in each animation to avoid
	 * popping, and animations should have same duration when synchronizing.
	 */
	class AnimationTargetController : public Controller
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit AnimationTargetController(AnimationLayer, bool sync = true);

		/*----------+
		| modifiers |
		+----------*/

		void SetTarget(); // no target
		void SetTarget(const AnimationController &);
		void SetTarget(const std::shared_ptr<AnimationController> &);

		/*-------------+
		| update hooks |
		+-------------*/

		private:
		virtual void UpdateTargets(float deltaTime) = 0;
		virtual float FixDeltaTime(float deltaTime);

		/*--------------------------+
		| Controller implementation |
		+--------------------------*/

		void DoUpdate(float deltaTime);
		Frame DoGetFrame(const Frame &, const Frame &) const;

		/*-------------+
		| data members |
		+-------------*/

		bool sync;
		float playPosition;
		typedef std::vector<std::shared_ptr<AnimationController>> Targets;
		Targets targets;
	};
}}

#endif
