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

#ifndef    page_local_phys_controller_LipsyncController_hpp
#   define page_local_phys_controller_LipsyncController_hpp

#	include <string>

#	include "TargetController.hpp"

namespace page { namespace phys
{
	namespace attrib { class Pose; }

	/**
	 * Lip-sync controller.
	 *
	 * @note Expects a bind-pose with closed mouth and neutral expression.
	 */
	class LipsyncController : public TargetController
	{
		IMPLEMENT_CLONEABLE(LipsyncController, Controller)

		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit LipsyncController(const attrib::Pose &);

		/**
		 * Returns @c true if the given pose has the necessary bones to work
		 * with this controller.
		 */
		static bool IsCompatibleWith(const attrib::Pose &);

		/*----------+
		| modifiers |
		+----------*/

		void PushText(const std::string &);
		void PushPhonemes(const std::string &);

		/*-------+
		| update |
		+-------*/

		private:
		void UpdateTargets(float deltaTime);

		/*-------------+
		| data members |
		+-------------*/

		std::string phonemes;
		float phonemeDuration;
	};
}}

#endif
