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

#include <algorithm> // find
#include <cassert>
#include <cmath> // abs, signbit
#include <utility> // pair
#include <vector>
#include "../../../math/intersect.hpp" // CapsuleSegmentIntersect, SweptCircleSegmentIntersectWeightTangent
#include "../../../util/iterator/chain_iterator.hpp"
#include "../../../util/iterator/range.hpp"
#include "../../../util/Optional.hpp"
#include "../Collidable.hpp"

namespace page
{
	namespace phys
	{
		namespace detail
		{
			struct Collider
			{
				// construct
				Collider(Collidable &collidable) :
					collidable(&collidable),
					position(Swizzle(collidable.GetLastPosition(), 0, 2)),
					force(Swizzle(collidable.GetPosition(), 0, 2) - position),
					direction(force), dirty(true) {}

				Collidable *collidable;
				math::Vector<2> position, force, direction;
				util::Optional<math::Vector<2>> constraint;
				res::TrackCrossings crossings;
				bool dirty;
			};

			struct EdgeSegment : std::pair<math::Vector<2>, math::Vector<2>>
			{
				typedef std::pair<math::Vector<2>, math::Vector<2>> Base;

				// construct
				EdgeSegment(const math::Vector<2> &a, const math::Vector<2> &b)
				{
					if (a.x < b.x || a.x == b.x && a.y < b.y)
					{
						first = a;
						second = b;
					}
					else
					{
						first = b;
						second = a;
					}
				}

				using Base::first;
				using Base::second;
			};
			inline bool operator ==(const EdgeSegment &a, const EdgeSegment &b)
			{
				return All(a.first == b.first && a.second == b.second);
			}

			struct EdgeTest
			{
				// construct
				EdgeTest(const EdgeSegment &segment, bool touching) :
					segment(segment), touching(touching) {}

				EdgeSegment segment;
				bool touching;
			};
			inline bool operator ==(const EdgeTest &et, const EdgeSegment &es)
			{
				return et.segment == es;
			}

			struct FaceEdge
			{
				// construct
				FaceEdge(const res::Track::Face &face, unsigned edge) :
					face(&face), edge(edge) {}

				const res::Track::Face *face;
				unsigned edge;
			};
			inline bool operator ==(const FaceEdge &a, const FaceEdge &b)
			{
				return a.face == b.face && a.edge == b.edge;
			}

			// FIXME: GCC 4.2.1 issues an incorrect warning when Collision is
			// defined in a function because it has a dependency on the
			// anonymous namespace (bug #29365); it has been fixed in GCC 4.3;
			// after switching to GCC 4.3, this can all be moved back into
			// UpdateCollidables
			typedef std::vector<Collider> Colliders;
			struct Collision
			{
				// construct
				Collision() : mu(1) {}

				float mu;
				Colliders::iterator collider;
				math::Vector<2> tangent;
			};
		}

		// update
		template <typename Iterator> void UpdateCollidables(Iterator first, Iterator last)
		{
			using namespace detail;
			// build list of potential colliders
			Colliders activeColliders, passiveColliders;
			for (const auto &collidable : util::make_range(first, last))
			{
				if (collidable.HasTrackFace())
				{
					switch (collidable.GetType())
					{
						case Collidable::active:
						if (Any(
							Swizzle(collidable.GetPosition(), 0, 2) !=
							Swizzle(collidable.GetLastPosition(), 0, 2)))
						{
							activeColliders.push_back(collidable);
							break;
						}
						case Collidable::passive:
						passiveColliders.push_back(collidable);
						break;
					}
				}
			}
			for (;;)
			{
				// build crossed track face edge lists
				auto colliders(util::make_range(
					// HACK: using constructor with explicit template parameter
					// instead of factory function to get around GCC 4.6.1 bug
					// related to std::initializer_list
					util::chain_iterator<typename Colliders::iterator>({
						util::make_range(activeColliders.begin(), activeColliders.end())},
						passiveColliders.begin()),
					util::make_chain_iterator(passiveColliders.end())));
				for (auto &collider : colliders)
				{
					if (collider.dirty)
					{
						collider.crossings = GetCrossings(
							collider.collidable->GetTrackFace(),
							collider.position,
							collider.position + collider.direction);
						collider.dirty = false;
					}
				}
				// find earliest collision
				Collision collision;
				for (Colliders::iterator collider(activeColliders.begin()); collider != activeColliders.end(); ++collider)
				{
					// build edge lists
					typedef std::vector<EdgeTest> EdgeTests;
					EdgeTests edgeTests;
					typedef std::vector<FaceEdge> FaceEdges;
					FaceEdges faceEdges;
					// insert initial track face edges
					{
						const res::Track::Face &face(collider->collidable->GetTrackFace());
						for (unsigned i = 0; i < 3; ++i)
						{
							EdgeSegment edgeSegment(
								Swizzle(face.vertices[i], 0, 2),
								Swizzle(face.vertices[(i + 1) % 3], 0, 2));
							EdgeTests::const_iterator edgeTest(std::find(
								edgeTests.begin(), edgeTests.end(), edgeSegment));
							bool touching;
							if (edgeTest == edgeTests.end())
							{
								touching = CapsuleSegmentIntersect(
									collider->position,
									collider->position + collider->direction,
									collider->collidable->GetRadius(),
									edgeSegment.first,
									edgeSegment.second);
								edgeTests.push_back(EdgeTest(edgeSegment, touching));
							}
							else touching = edgeTest->touching;
							if (touching) faceEdges.push_back(FaceEdge(face, i));
						}
					}
					// insert crossed edges
					{
						faceEdges.reserve(faceEdges.size() + collider->crossings.size());
						const res::Track::Face *face = &collider->collidable->GetTrackFace();
						for (res::TrackCrossings::const_iterator crossing(collider->crossings.begin()); crossing != collider->crossings.end(); ++crossing)
						{
							EdgeSegment edgeSegment(
								Swizzle(face->vertices[*crossing], 0, 2),
								Swizzle(face->vertices[(*crossing + 1) % 3], 0, 2));
							if (std::find(edgeTests.begin(), edgeTests.end(), edgeSegment) == edgeTests.end())
								edgeTests.push_back(EdgeTest(edgeSegment, true));
							FaceEdge faceEdge(*face, *crossing);
							if (std::find(faceEdges.begin(), faceEdges.end(), faceEdge) == faceEdges.end())
								faceEdges.push_back(faceEdge);
							face = face->neighbours[*crossing];
						}
					}
					// expand edge list and look for collisions
					for (FaceEdges::const_iterator faceEdge(faceEdges.begin()); faceEdge != faceEdges.end(); ++faceEdge)
					{
						// check collision
						if (!faceEdge->face->neighbours[faceEdge->edge] &&
							PerpDot(
								Swizzle(faceEdge->face->vertices[(faceEdge->edge + 1) % 3], 0, 2) -
								Swizzle(faceEdge->face->vertices[faceEdge->edge], 0, 2),
								collider->direction) < 0)
						{
							std::pair<float, math::Vector<2>> mutan(
								SweptCircleSegmentIntersectWeightTangent(
									collider->position,
									collider->position + collider->direction,
									collider->collidable->GetRadius(),
									Swizzle(faceEdge->face->vertices[faceEdge->edge], 0, 2),
									Swizzle(faceEdge->face->vertices[(faceEdge->edge + 1) % 3], 0, 2)));
							// HACK: although Near(mutan.first, 0) would be
							// preferable, it allowed a Collider to pass through
							// an inset corner, probably due to precision errors
							if (mutan.first < collision.mu && mutan.first > -.001f)
							{
								if (collider->constraint)
								{
									float
										newConstraint(Dot(collider->force, mutan.second)),
										oldConstraint(Dot(collider->force, *collider->constraint));
									if (std::signbit(newConstraint) != std::signbit(oldConstraint) ||
										std::abs(newConstraint) < std::abs(oldConstraint))
										goto Collision;
								}
								else
								{
									Collision:
									collision.mu = mutan.first;
									collision.collider = collider;
									collision.tangent = mutan.second;
								}
							}
						}
						// expand into sibling edges
						for (unsigned i = 0; i < 3; ++i)
						{
							FaceEdge siblingFaceEdge(*faceEdge->face, i);
							FaceEdges::const_iterator iter(std::find(
								faceEdges.begin(), faceEdges.end(), siblingFaceEdge));
							if (iter == faceEdges.end())
							{
								EdgeSegment edgeSegment(
									Swizzle(faceEdge->face->vertices[i], 0, 2),
									Swizzle(faceEdge->face->vertices[(i + 1) % 3], 0, 2));
								EdgeTests::const_iterator edgeTest(std::find(
									edgeTests.begin(), edgeTests.end(), edgeSegment));
								bool touching;
								if (edgeTest == edgeTests.end())
								{
									touching = CapsuleSegmentIntersect(
										collider->position,
										collider->position + collider->direction,
										collider->collidable->GetRadius(),
										edgeSegment.first,
										edgeSegment.second);
									edgeTests.push_back(EdgeTest(edgeSegment, touching));
								}
								else touching = edgeTest->touching;
								if (touching)
								{
									FaceEdges::difference_type index = faceEdge - faceEdges.begin();
									faceEdges.push_back(siblingFaceEdge);
									faceEdge = faceEdges.begin() + index;
								}
							}
						}
						// expand into neighbouring face edges
						if (faceEdge->face->neighbours[faceEdge->edge])
						{
							const res::Track::Face &neighbour(*faceEdge->face->neighbours[faceEdge->edge]);
							for (unsigned i = 0; i < 3; ++i)
							{
								FaceEdge neighbourFaceEdge(neighbour, i);
								FaceEdges::const_iterator iter(std::find(
									faceEdges.begin(), faceEdges.end(), neighbourFaceEdge));
								if (iter == faceEdges.end())
								{
									EdgeSegment edgeSegment(
										Swizzle(neighbour.vertices[i], 0, 2),
										Swizzle(neighbour.vertices[(i + 1) % 3], 0, 2));
									EdgeTests::const_iterator edgeTest(std::find(
										edgeTests.begin(), edgeTests.end(), edgeSegment));
									bool touching;
									if (edgeTest == edgeTests.end())
									{
										touching = CapsuleSegmentIntersect(
											collider->position,
											collider->position + collider->direction,
											collider->collidable->GetRadius(),
											edgeSegment.first,
											edgeSegment.second);
										edgeTests.push_back(EdgeTest(edgeSegment, touching));
									}
									else touching = edgeTest->touching;
									if (touching)
									{
										FaceEdges::difference_type index = faceEdge - faceEdges.begin();
										faceEdges.push_back(neighbourFaceEdge);
										faceEdge = faceEdges.begin() + index;
									}
								}
							}
						}
					}
					// check active collider collision
					// FIXME: implement
					// check passive collider collision
					// FIXME: implement
				}
				// evaluate collision
				if (collision.mu < 1)
				{
					if (collision.mu)
					{
						// step to point of collision
						for (Colliders::iterator collider(activeColliders.begin()); collider != activeColliders.end();)
						{
							collider->position  += collider->direction * collision.mu;
							collider->force     -= collider->force     * collision.mu;
							collider->direction -= collider->direction * collision.mu;
							// update track binding
							const res::Track::Face *face = &collider->collidable->GetTrackFace();
							for (res::TrackCrossings::iterator crossing(collider->crossings.begin());; ++crossing)
							{
								// FIXME: if we had more information, we
								// wouldn't have to do these dot products; for
								// instance, if we had the mu for the point of
								// crossing, although you'd have to rescale it
								// every iteration as the simulation time
								// progresses
								if (crossing == collider->crossings.end() ||
									PerpDot(
										collider->position -
										Swizzle(face->vertices[*crossing], 0, 2),
										Swizzle(face->vertices[(*crossing + 1) % 3], 0, 2) -
										Swizzle(face->vertices[*crossing], 0, 2)) <= 0)
								{
									collider->crossings.erase(collider->crossings.begin(), crossing);
									break;
								}
								face = face->neighbours[*crossing];
							}
							collider->collidable->SetTrackFace(face);
							if (!face)
							{
								collider->collidable->SetTracklessPosition(collider->position);
								collider = activeColliders.erase(collider);
								continue;
							}
							++collider;
						}
					}
					// introduce constraint
					// FIXME: constraint should be applied to other collider as
					// well, if one was involved, whether active or passive
					if (collision.collider->constraint &&
						std::signbit(Dot(collision.collider->force, collision.tangent)) !=
						std::signbit(Dot(collision.collider->force, *collision.collider->constraint)))
					{
						// wedge
						collision.collider->collidable->SetTracklessPosition(collision.collider->position);
						activeColliders.erase(collision.collider);
					}
					else
					{
						// assert new constraint is more constraining
						// HACK: accept equal constraint to allow for floating-
						// point imprecision
						assert(!collision.collider->constraint ||
							std::abs(Dot(collision.collider->force, collision.tangent)) <=
							std::abs(Dot(collision.collider->force, *collision.collider->constraint)));
						// apply new constraint to direction
						collision.collider->direction = Proj(collision.collider->force, collision.tangent);
						collision.collider->constraint = collision.tangent;
						collision.collider->dirty = true;
					}
				}
				else
				{
					auto colliders(util::make_range(
						// HACK: using constructor with explicit template
						// parameter instead of factory function to get around
						// GCC 4.6.1 bug related to std::initializer_list
						util::chain_iterator<typename Colliders::iterator>({
							util::make_range(activeColliders.begin(), activeColliders.end())},
							passiveColliders.begin()),
						util::make_chain_iterator(passiveColliders.end())));
					for (auto &collider : colliders)
					{
						// FIXME: given more crossing information, such as the
						// face pointer, we wouldn't have to iterate
						const res::Track::Face *face = &collider.collidable->GetTrackFace();
						for (res::TrackCrossings::const_iterator crossing(collider.crossings.begin()); crossing != collider.crossings.end(); ++crossing)
							face = face->neighbours[*crossing];
						collider.collidable->SetTrackFace(face);
						collider.collidable->SetTracklessPosition(collider.position + collider.direction);
					}
					break;
				}
			}
		}
	}
}
