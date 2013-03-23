/**
 * @section license
 *
 * Copyright (c) 2006-2013 David Osborn
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
