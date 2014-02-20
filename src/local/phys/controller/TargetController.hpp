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

#ifndef    page_local_phys_controller_TargetController_hpp
#   define page_local_phys_controller_TargetController_hpp

#	include <vector>

#	include "Controller.hpp"

namespace page { namespace phys
{
	/**
	 * Interpolated frame matching controller.
	 *
	 * @note Similar attributes should be defined in each frame to avoid
	 * popping.
	 */
	class TargetController : public Controller
	{
		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit TargetController(AnimationLayer, float fadeDuration = 1);

		/*----------+
		| modifiers |
		+----------*/

		protected:
		void SetTarget(const Frame &);
		void SetTarget(const Frame &, float fadeDuration);

		/*-------------+
		| update hooks |
		+-------------*/

		private:
		virtual void UpdateTargets(float deltaTime) = 0;

		/*--------------------------+
		| Controller implementation |
		+--------------------------*/

		void DoUpdate(float deltaTime);
		Frame DoGetFrame(const Frame &, const Frame &) const;

		/*-------------+
		| data members |
		+-------------*/

		float fadeDuration;
		struct Target
		{
			Frame frame;
			float opacity, fadeDuration;
		};
		typedef std::vector<Target> Targets;
		Targets targets;
	};
}}

#endif
