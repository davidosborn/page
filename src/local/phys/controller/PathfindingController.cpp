#include "../Body.hpp" // Body->{Collidable,PositionOrientation}
#include "../mixin/Collidable.hpp" // Collidable::GetTrack
#include "PathfindingController.hpp"

namespace page
{
	namespace phys
	{
		// construct
		PathfindingController::PathfindingController(const Body &controlled) :
			LocomotionController(controlled), controlled(controlled) {}

		// modifiers
		void PathfindingController::Goto(const math::Vec3 &targetPosition)
		{
			if (const res::Track::Face *targetTrackFace =
				GetBestFace(controlled.GetTrack(), targetPosition))
				Goto(*targetTrackFace, Swizzle(targetPosition, 0, 2));
			else SetTarget(targetPosition);
		}
		void PathfindingController::Goto(const res::Track::Face &target)
		{
			Goto(target, Swizzle(GetCenter(target), 0, 2));
		}
		void PathfindingController::Goto(const res::Track::Face &targetTrackFace, const math::Vec2 &targetPosition)
		{
			path = FindPath(controlled.GetTrackFace(), targetTrackFace, controlled.GetRadius());
		}
		void PathfindingController::Stop()
		{
			SetTarget();
			while (!path.empty()) path.pop();
		}

		// update hooks
		void PathfindingController::UpdateLocomotion()
		{
			if (!path.empty() && !HasTarget())
			{
				// FIXME: set orientation toward next path node
				SetTarget(GetCenter(*path.front()));
				path.pop();
			}
		}
	}
}
