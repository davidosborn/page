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

#include <algorithm> // find, max, pop_heap, push_heap, reverse
#include <cassert>
#include <deque>
#include <functional> // bind, less
#include <iostream> // clog

#include "../../err/Exception.hpp"
#include "../../log/manip.hpp" // Warning
#include "../../math/float.hpp" // Near
#include "../../math/interp.hpp" // Bilerp
#include "../../math/intersect.hpp" // ClosestPointOnLine, LineIntersectSecondWeight, PointInTriangle{,Edges}
#include "../../util/functional/member.hpp" // make_member_of
#include "../../util/iterator/member_iterator.hpp"
#include "Registry.hpp" // REGISTER_TYPE
#include "Track.hpp"

namespace page
{
	namespace res
	{
		namespace
		{
			// pathfinding
			struct PathNode
			{
				explicit PathNode(const Track::Face *face, const Track::Face *parent = 0, float cost = 0) :
					face(face), parent(parent), cost(cost) {}

				const Track::Face *face, *parent;
				float cost;
			};
			typedef std::vector<PathNode> PathNodes;

			// pathfinding identity search
			inline PathNodes::iterator Find(PathNodes &nodes, const Track::Face &neighbour)
			{
				return std::find(
					util::make_member_iterator(nodes.begin(), &PathNode::face),
					util::make_member_iterator(nodes.end(),   &PathNode::face),
					&neighbour).base();
			}
			inline PathNodes::const_iterator Find(const PathNodes &nodes, const Track::Face &neighbour)
			{
				return std::find(
					util::make_member_iterator(nodes.begin(), &PathNode::face),
					util::make_member_iterator(nodes.end(),   &PathNode::face),
					&neighbour).base();
			}

			// pathfinding priority queue
			inline PathNode &Top(PathNodes &nodes)
			{
				return nodes.front();
			}
			inline const PathNode &Top(const PathNodes &nodes)
			{
				return nodes.front();
			}
			inline void Push(PathNodes &nodes, const PathNode &node)
			{
				nodes.push_back(node);
				using std::bind;
				using namespace std::placeholders;
				std::push_heap(nodes.begin(), nodes.end(),
					bind(std::less<float>(),
						bind(util::make_member_of(&PathNode::cost), _1),
						bind(util::make_member_of(&PathNode::cost), _2)));
			}
			inline PathNode Pop(PathNodes &nodes)
			{
				PathNode node(Top(nodes));
				using std::bind;
				using namespace std::placeholders;
				std::pop_heap(nodes.begin(), nodes.end(),
					bind(std::less<float>(),
						bind(util::make_member_of(&PathNode::cost), _1),
						bind(util::make_member_of(&PathNode::cost), _2)));
				nodes.pop_back();
				return node;
			}
		}

		// track construct/copy
		Track::Track() {}
		Track::Track(const Track &other) :
			faces(other.faces)
		{
			// remap neighbours
			for (Faces::iterator face(faces.begin()); face != faces.end(); ++face)
				for (Face::Neighbours::iterator neighbour(face->neighbours.begin()); neighbour != face->neighbours.end(); ++neighbour)
					*neighbour = &*faces.begin() + (*neighbour - &*other.faces.begin());
		}
		Track &Track::operator =(const Track &other)
		{
			faces = other.faces;
			// remap neighbours
			for (Faces::iterator face(faces.begin()); face != faces.end(); ++face)
				for (Face::Neighbours::iterator neighbour(face->neighbours.begin()); neighbour != face->neighbours.end(); ++neighbour)
					*neighbour = &*faces.begin() + (*neighbour - &*other.faces.begin());
		}

		// face functions
		math::Vector<3> GetNormal(const Track::Face &face)
		{
			return Norm(Cross(
				face.vertices[1] - face.vertices[0],
				face.vertices[2] - face.vertices[0]));
		}
		math::Vector<3> GetCenter(const Track::Face &face)
		{
			return (face.vertices[0] + face.vertices[1] + face.vertices[2]) / 3;
		}
		int GetNeighbourEdge(const Track::Face &face, unsigned edge)
		{
			assert(edge < face.neighbours.size());
			const Track::Face &neighbour(*face.neighbours[edge]);
			for (unsigned i = 0; i < neighbour.neighbours.size(); ++i)
				if (neighbour.neighbours[i] == &face) return i;
			return -1;
		}

		// binding
		const Track::Face *GetBestFace(const Track &track, const math::Vector<3> &pos)
		{
			const Track::Face *selFace = 0;
			float selHeight;
			for (Track::Faces::const_iterator face(track.faces.begin()); face != track.faces.end(); ++face)
				if (PointInTriangle(
					Swizzle(pos, 0, 2),
					Swizzle(face->vertices[0], 0, 2),
					Swizzle(face->vertices[1], 0, 2),
					Swizzle(face->vertices[2], 0, 2)))
					if (selFace)
					{
						math::Vector<2> bc(Barycentric(
							Swizzle(face->vertices[0], 0, 2),
							Swizzle(face->vertices[1], 0, 2),
							Swizzle(face->vertices[2], 0, 2),
							Swizzle(pos, 0, 2)));
						float height = math::Bilerp(face->vertices[0].y, face->vertices[1].y, face->vertices[2].y, bc.x, bc.y);
						if (height > selHeight && (height <= pos.y || math::Near(height, pos.y)))
						{
							selFace = &*face;
							selHeight = height;
						}
					}
					else selFace = &*face;
			return selFace;
		}

		// edge crossing
		TrackCrossings GetCrossings(const Track::Face &originFace, const math::Vector<2> &origin, const math::Vector<2> &target)
		{
			TrackCrossings crossings;
			math::Vector<2> position(origin);
			for (const Track::Face *face = &originFace;;)
			{
				math::Vector<2> vertices[] =
				{
					Swizzle(face->vertices[0], 0, 2),
					Swizzle(face->vertices[1], 0, 2),
					Swizzle(face->vertices[2], 0, 2)
				};
				// FIXME: this assertion is too precise
/*				assert(math::PointInTriangle(position,
					vertices[0], vertices[1], vertices[2]));*/
				math::Vector<3, bool> outside(!PointInTriangleEdges(
					target, vertices[0], vertices[1], vertices[2]));
				if (!Any(outside)) break;
				position = ClosestPointOnLine(position, target,
					(vertices[0] + vertices[1] + vertices[2]) / 3);
				unsigned crossing;
				if (outside.x + outside.y + outside.z > 1)
				{
					if (outside.x)
					{
						if (outside.y)
							crossing = math::LineIntersectSecondWeight(
								position, target, vertices[1], vertices[2]) >= 0;
						else
							crossing = 2 * (math::LineIntersectSecondWeight(
								position, target, vertices[0], vertices[1]) >= 0);
					}
					else
						crossing = 1 + (math::LineIntersectSecondWeight(
							position, target, vertices[2], vertices[0]) >= 0);
				}
				else crossing = outside.x ? 0 : outside.y ? 1 : 2;
				crossings.push_back(crossing);
				if (!(face = face->neighbours[crossing])) break;
			}
			return crossings;
		}

		// pathfinding
		TrackPath FindPath(const Track::Face &source, const Track::Face &target, float radius)
		{
			// FIXME: perform collision detection with track faces to ensure
			// that a collider with the given radius can make it through
			PathNodes open(1, PathNode(&source)), closed;
			while (Top(open).face != &target)
			{
				PathNode node(Pop(open));
				Push(closed, node);
				for (Track::Face::Neighbours::const_iterator iter(node.face->neighbours.begin()); iter != node.face->neighbours.end(); ++iter)
				{
					const Track::Face &neighbour(**iter);
					float cost = node.cost + Len(GetCenter(neighbour) - GetCenter(*node.face));
					PathNodes::iterator iter(Find(open, neighbour));
					if (iter == open.end())
					{
						PathNodes::iterator iter(Find(closed, neighbour));
						if (iter == closed.end())
							Push(open, PathNode(&neighbour, node.face, cost));
						else if (cost < iter->cost)
							closed.erase(iter);
					}
					else if (cost < iter->cost)
						open.erase(iter);
				}
			}
			// build result by walking back through parents
			std::deque<const Track::Face *> path;
			for (const Track::Face *face = Top(open).face; face != &source;)
			{
				path.push_front(face);
				PathNodes::const_iterator iter(Find(open, *face));
				assert(iter != open.end());
				face = iter->parent;
				assert(face);
			}
			return TrackPath(path);
		}

		// validation
		void CheckContinuity(const Track &track)
		{
			for (Track::Faces::const_iterator face(track.faces.begin()); face != track.faces.end(); ++face)
				for (unsigned i = 0; i < 3; ++i)
					if (face->neighbours[i])
					{
						const Track::Face &neighbour(*face->neighbours[i]);
						struct math::Vector<2> vertices[2] =
						{
							Swizzle(face->vertices[i], 0, 2),
							Swizzle(face->vertices[(i + 1) % 3], 0, 2)
						};
						unsigned matches = 0;
						for (Track::Face::Vertices::const_iterator neighbourVertex(neighbour.vertices.begin()); neighbourVertex != neighbour.vertices.end(); ++neighbourVertex)
						{
							if (All(Swizzle(*neighbourVertex, 0, 2) == vertices[0]) ||
								All(Swizzle(*neighbourVertex, 0, 2) == vertices[1])) ++matches;
						}
						if (matches < 2)
							THROW((err::Exception<err::ResModuleTag, err::FormatTag>("mismatched vertex in neighbouring face")))
					}
		}
		void CheckSlope(const Track &track)
		{
			enum Slope
			{
				goodSlope,
				steepSlope,
				verticalSlope,
				backSlope
			};
			int slope = goodSlope;
			for (Track::Faces::const_iterator face(track.faces.begin()); face != track.faces.end(); ++face)
			{
				float dot = Dot(GetNormal(*face), math::UpVector<3>());
				if (math::Near(dot, 0.f)) slope = std::max<int>(slope, verticalSlope);
				else if (dot < 0.f) slope = std::max<int>(slope, backSlope);
				else if (dot < .5f) slope = std::max<int>(slope, steepSlope);
			}
			switch (slope)
			{
				case steepSlope: std::clog << log::Warning << "contains steep faces" << std::endl; break;
				case verticalSlope: std::clog << log::Warning << "contains vertical faces" << std::endl; break;
				case backSlope: std::clog << log::Warning << "contains back-faces" << std::endl; break;
			}
		}

		void PostLoadTrack(Track &track)
		{
			CheckContinuity(track);
			CheckSlope(track);
		}

		REGISTER_TYPE(Track, "track", PostLoadTrack)
	}
}
