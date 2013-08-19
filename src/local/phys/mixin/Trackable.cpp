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
		void Trackable::SetPosition(const math::Vec2 &position)
		{
			if (trackFace)
			{
				math::Vec2 bc(Barycentric(
					Swizzle(trackFace->vertices[0], 0, 2),
					Swizzle(trackFace->vertices[1], 0, 2),
					Swizzle(trackFace->vertices[2], 0, 2), position));
				float height = math::Bilerp(
					trackFace->vertices[0].y,
					trackFace->vertices[1].y,
					trackFace->vertices[2].y, bc.x, bc.y);
				SetPosition(math::Vec3(position.x, height, position.y));
			}
			else SetTracklessPosition(position);
		}
		void Trackable::SetTracklessPosition(const math::Vec2 &position)
		{
			SetPosition(math::Vec3(position.x, GetPosition().y, position.y));
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
