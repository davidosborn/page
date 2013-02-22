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

#ifndef    page_local_phys_controller_GaitController_hpp
#   define page_local_phys_controller_GaitController_hpp

#	include "../../cache/proxy/Proxy.hpp" // Proxy::Instance
#	include "../../res/type/Gait.hpp" // Gait::State
#	include "AnimationTargetController.hpp"

namespace page
{
	namespace phys
	{
		namespace attrib { class PositionOrientation; }

		struct GaitController : AnimationTargetController
		{
			// construct
			GaitController(
				const attrib::PositionOrientation &,
				const cache::Proxy<res::Gait> &);

			// clone
			GaitController *Clone() const;

			private:
			enum State
			{
				noState,
				runState,
				sneakState,
				turnState,
				walkState
			};

			// modifiers
			void SetState(State);
			void SetState(const res::Gait::State &);

			// update hooks
			void UpdateTargets(float deltaTime);
			float FixDeltaTime(float deltaTime);

			const attrib::PositionOrientation &body;
			cache::Proxy<res::Gait>::Instance gait;
			State state;
			float stride;
		};
	}
}

#endif
