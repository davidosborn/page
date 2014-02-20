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

#ifndef    page_local_phys_controller_PathfindingController_hpp
#   define page_local_phys_controller_PathfindingController_hpp

#	include "../../res/type/Track.hpp" // Track::Face, TrackPath
#	include "LocomotionController.hpp"

namespace page { namespace phys
{
	class Body;
	class Collidable;

	/**
	 * Pathfinding controller.
	 *
	 * Supports targeted locomotion across a track.
	 */
	class PathfindingController : public LocomotionController
	{
		IMPLEMENT_CLONEABLE(PathfindingController, Controller)

		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit PathfindingController(const Body &);

		/*----------+
		| modifiers |
		+----------*/

		void Goto(const math::Vec3 &);
		void Goto(const res::Track::Face &);
		void Goto(const res::Track::Face &, const math::Vec2 &);
		void Stop();

		/*-------------+
		| update hooks |
		+-------------*/

		private:
		void UpdateLocomotion();

		/*-------------+
		| data members |
		+-------------*/

		const Collidable &controlled;
		res::TrackPath path;
	};
}}

#endif
