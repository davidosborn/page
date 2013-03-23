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

#include <algorithm> // find
#include <cassert>

#include "../../math/interp.hpp" // Bilerp
#include "Trackable.hpp"

namespace page
{
	namespace phys
	{
		// construct
		Trackable::Trackable() : track(0), trackFace(0) {}
		Trackable::Trackable(const res::Track &track) :
			track(&track), trackFace(0) {}

		// track binding state
		bool Trackable::HasTrack() const
		{
			return track;
		}
		bool Trackable::HasTrackFace() const
		{
			return trackFace;
		}
		const res::Track &Trackable::GetTrack() const
		{
			assert(track);
			return *track;
		}
		const res::Track::Face &Trackable::GetTrackFace() const
		{
			assert(trackFace);
			return *trackFace;
		}
		unsigned Trackable::GetTrackFaceIndex() const
		{
			assert(trackFace);
			return trackFace - &*track->faces.begin();
		}

		// track binding modifiers
		void Trackable::SetTrack()
		{
			SetTrack(0);
		}
		void Trackable::SetTrack(const res::Track &track)
		{
			SetTrack(&track);
		}
		void Trackable::SetTrack(const res::Track *track)
		{
			if (track != this->track)
			{
				trackFace = 0;
				if ((this->track = track) && BindTrackFace())
					SetPosition(Swizzle(GetPosition(), 0, 2));
			}
		}
		void Trackable::SetTrackFace()
		{
			SetTrackFace(0);
		}
		void Trackable::SetTrackFace(const res::Track::Face &face)
		{
			SetTrackFace(&face);
		}
		void Trackable::SetTrackFace(const res::Track::Face *face)
		{
#ifndef NDEBUG
			if (face)
			{
				assert(track);
				// assert face is in track
				for (res::Track::Faces::const_iterator iter(track->faces.begin());; ++iter)
				{
					if (&*iter == face) break;
					if (iter == track->faces.end())
						assert(!"face not in track");
				}
			}
#endif
			trackFace = face;
		}
		bool Trackable::BindTrackFace()
		{
			assert(track);
			return trackFace = GetBestFace(*track, GetPosition());
		}

		// position modifiers
		void Trackable::SetPosition(const math::Vector<2> &position)
		{
			if (trackFace)
			{
				math::Vector<2> bc(Barycentric(
					Swizzle(trackFace->vertices[0], 0, 2),
					Swizzle(trackFace->vertices[1], 0, 2),
					Swizzle(trackFace->vertices[2], 0, 2), position));
				float height = math::Bilerp(
					trackFace->vertices[0].y,
					trackFace->vertices[1].y,
					trackFace->vertices[2].y, bc.x, bc.y);
				SetPosition(math::Vector<3>(position.x, height, position.y));
			}
			else SetTracklessPosition(position);
		}
		void Trackable::SetTracklessPosition(const math::Vector<2> &position)
		{
			SetPosition(math::Vector<3>(position.x, GetPosition().y, position.y));
		}

		// deferred initialization
		void Trackable::Init()
		{
			if (track && BindTrackFace())
			{
				SetPosition(Swizzle(GetPosition(), 0, 2));
				BakeTransform();
			}
		}
	}
}
