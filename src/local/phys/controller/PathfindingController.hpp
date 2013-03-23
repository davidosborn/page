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

// pathfinding controller
// supports targeted locomotion across a track

#ifndef    page_local_phys_controller_PathfindingController_hpp
#   define page_local_phys_controller_PathfindingController_hpp

#	include "../../res/type/Track.hpp" // Track::Face, TrackPath
#	include "LocomotionController.hpp"

namespace page { namespace phys
{
	class Body;
	class Collidable;

	class PathfindingController :
		public LocomotionController,
		public virtual util::Cloneable<PathfindingController, LocomotionController>
	{
		public:
		// construct
		explicit PathfindingController(const Body &);

		// modifiers
		void Goto(const math::Vector<3> &);
		void Goto(const res::Track::Face &);
		void Goto(const res::Track::Face &, const math::Vector<2> &);
		void Stop();

		private:
		// update hooks
		void UpdateLocomotion();

		const Collidable &controlled;
		res::TrackPath path;
	};
}}

#endif
