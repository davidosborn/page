#include <cassert>

#include "../Trackable.hpp"

namespace page
{
	namespace phys
	{
		// update
		template <typename Iterator> void UpdateTrackables(Iterator first, Iterator last)
		{
			for (Iterator trackable(first); trackable != last; ++trackable)
			{
				if (Any(trackable->GetPosition() != trackable->GetLastPosition()))
				{
					if (trackable->HasTrackFace() || trackable->HasTrack() && trackable->BindTrackFace())
					{
						res::TrackCrossings crossings(GetCrossings(trackable->GetTrackFace(),
							Swizzle(trackable->GetLastPosition(), 0, 2),
							Swizzle(trackable->GetPosition(), 0, 2)));
						const res::Track::Face *face = &trackable->GetTrackFace();
						for (res::TrackCrossings::const_iterator crossing(crossings.begin()); crossing != crossings.end(); ++crossing)
						{
							assert(face);
							face = face->neighbours[*crossing];
						}
						if (face)
						{
							trackable->SetTrackFace(*face);
							SetPosition:
							trackable->SetPosition(Swizzle(trackable->GetPosition(), 0, 2));
						}
						else if (trackable->BindTrackFace()) goto SetPosition;
					}
				}
			}
		}
	}
}
