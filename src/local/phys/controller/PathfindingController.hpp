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

		public:
		// construct
		explicit PathfindingController(const Body &);

		// modifiers
		void Goto(const math::Vec3 &);
		void Goto(const res::Track::Face &);
		void Goto(const res::Track::Face &, const math::Vec2 &);
		void Stop();

		private:
		// update hooks
		void UpdateLocomotion();

		const Collidable &controlled;
		res::TrackPath path;
	};
}}

#endif
