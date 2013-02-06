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
				typedef std::array<math::Vector<3>, 3> Vertices;
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
		math::Vector<3> GetNormal(const Track::Face &);
		math::Vector<3> GetCenter(const Track::Face &);
		int GetNeighbourEdge(const Track::Face &, unsigned edge);

		// binding
		const Track::Face *GetBestFace(const Track &, const math::Vector<3> &);

		// edge crossing
		typedef std::vector<unsigned> TrackCrossings;
		TrackCrossings GetCrossings(const Track::Face &originFace,
			const math::Vector<2> &origin, const math::Vector<2> &target);

		// pathfinding
		typedef std::queue<const Track::Face *> TrackPath;
		TrackPath FindPath(const Track::Face &source, const Track::Face &target, float radius = 0);

		// validation
		void CheckContinuity(const Track &);
		void CheckSlope(const Track &);
	}
}

#endif
