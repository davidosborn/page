#ifndef    page_local_res_type_Track_hpp
#   define page_local_res_type_Track_hpp

#	include <array>
#	include <queue>
#	include <vector>

#	include "../../math/Vector.hpp"

namespace page
{
	namespace res
	{
		struct Track
		{
			// construct/copy
			Track();
			Track(const Track &);
			Track &operator =(const Track &);

			struct Face
			{
				typedef std::array<math::Vec3, 3> Vertices;
				Vertices vertices;
				// NOTE: winged face data structure
				typedef std::array<const Face *, 3> Neighbours;
				Neighbours neighbours;
				// FIXME: providing the edge index for each neighbouring face
				// which links back to this face would optimize certain cases
			};
			typedef std::vector<Face> Faces;
			Faces faces;
		};

		// face functions
		math::Vec3 GetNormal(const Track::Face &);
		math::Vec3 GetCenter(const Track::Face &);
		int GetNeighbourEdge(const Track::Face &, unsigned edge);

		// binding
		const Track::Face *GetBestFace(const Track &, const math::Vec3 &);

		// edge crossing
		typedef std::vector<unsigned> TrackCrossings;
		TrackCrossings GetCrossings(const Track::Face &originFace,
			const math::Vec2 &origin, const math::Vec2 &target);

		// pathfinding
		typedef std::queue<const Track::Face *> TrackPath;
		TrackPath FindPath(const Track::Face &source, const Track::Face &target, float radius = 0);

		// validation
		void CheckContinuity(const Track &);
		void CheckSlope(const Track &);
	}
}

#endif
