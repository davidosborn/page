#ifndef    page_math_intersect_hpp
#   define page_math_intersect_hpp

#	include <utility> // pair

#	include "fwd.hpp" // Plane
#	include "Vector.hpp"

namespace page { namespace math
{
	// closest point on line
	template <unsigned n, typename T> T
		ClosestPointOnLineWeight(
			const Vector<n, T> &, const Vector<n, T> &, // line
			const Vector<n, T> &); // point
	template <unsigned n, typename T> Vector<n, T>
		ClosestPointOnLine(
			const Vector<n, T> &, const Vector<n, T> &, // line
			const Vector<n, T> &); // point
	template <unsigned n, typename T> Vector<n, T>
		ClosestPointOnSegment(
			const Vector<n, T> &, const Vector<n, T> &, // line
			const Vector<n, T> &); // point

	// closest point on plane
	template <unsigned n, typename T> T
		ClosestPointOnPlaneWeight(
			const Plane<n, T> &, // plane
			const Vector<n, T> &); // point
	template <unsigned n, typename T> Vector<n, T>
		ClosestPointOnPlane(
			const Plane<n, T> &, // plane
			const Vector<n, T> &); // point

	// closest point between lines
	template <unsigned n, typename T> std::pair<T, T>
		ClosestPointSegmentWeight(
			const Vector<n, T> &, const Vector<n, T> &,  // line
			const Vector<n, T> &, const Vector<n, T> &); // line
	template <unsigned n, typename T> std::pair<Vector<n, T>, Vector<n, T>>
		ClosestPointSegment(
			const Vector<n, T> &, const Vector<n, T> &,  // line
			const Vector<n, T> &, const Vector<n, T> &); // line

	// point/line distance
	template <unsigned n, typename T> T
		PointSegmentSqrDist(
			const Vector<n, T> &, // point
			const Vector<n, T> &, const Vector<n, T> &); // line
	template <unsigned n, typename T> T
		PointSegmentDist(
			const Vector<n, T> &, // point
			const Vector<n, T> &, const Vector<n, T> &); // line

	// line/line distance
	template <unsigned n, typename T> T
		SegmentSqrDist(
			const Vector<n, T> &, const Vector<n, T> &,  // line
			const Vector<n, T> &, const Vector<n, T> &); // line
	template <unsigned n, typename T> T
		SegmentDist(
			const Vector<n, T> &, const Vector<n, T> &,  // line
			const Vector<n, T> &, const Vector<n, T> &); // line

	// point/triangle intersection
	template <typename T> bool
		PointInTriangle(
			const Vector<2, T> &, // point
			const Vector<2, T> &, const Vector<2, T> &, const Vector<2, T> &); // triangle
	template <typename T> Vector<3, bool>
		PointInTriangleEdges(
			const Vector<2, T> &, // point
			const Vector<2, T> &, const Vector<2, T> &, const Vector<2, T> &); // triangle

	// line/line intersection
	// NOTE: lines are represented by two end-points
	template <typename T> T
		LineIntersectFirstWeight(
			const Vector<2, T> &, const Vector<2, T> &,  // line
			const Vector<2, T> &, const Vector<2, T> &); // line
	template <typename T> T
		LineIntersectSecondWeight(
			const Vector<2, T> &, const Vector<2, T> &,  // line
			const Vector<2, T> &, const Vector<2, T> &); // line
	template <typename T> std::pair<T, T>
		LineIntersectWeight(
			const Vector<2, T> &, const Vector<2, T> &,  // line
			const Vector<2, T> &, const Vector<2, T> &); // line
	template <typename T> Vector<2, T>
		LineIntersect(
			const Vector<2, T> &, const Vector<2, T> &,  // line
			const Vector<2, T> &, const Vector<2, T> &); // line
	template <typename T> std::pair<bool, Vector<2, T>>
		SegmentIntersect(
			const Vector<2, T> &, const Vector<2, T> &,  // line
			const Vector<2, T> &, const Vector<2, T> &); // line

	// line/circle intersection
	template <typename T> T
		LineCircleIntersectWeight(
			const Vector<2, T> &, const Vector<2, T> &, // line
			const Vector<2, T> &, T); // circle
	template <typename T> std::pair<bool, T>
		SegmentCircleIntersect(
			const Vector<2, T> &, const Vector<2, T> &, // line
			const Vector<2, T> &, T); // circle

	// capsule/line intersection
	template <typename T> bool
		CapsuleSegmentIntersect(
			const Vector<2, T> &, const Vector<2, T> &, T radius, // capsule
			const Vector<2, T> &, const Vector<2, T> &); // line

	// swept-circle/line intersection
	// NOTE: uses earliest point of intersection, even if already passed
	template <typename T> T
		SweptCircleLineIntersectFirstWeight(
			const Vector<2, T> &, const Vector<2, T> &, T radius, // swept circle
			const Vector<2, T> &, const Vector<2, T> &); // line
	template <typename T> std::pair<T, T>
		SweptCircleLineIntersectWeight(
			const Vector<2, T> &, const Vector<2, T> &, T radius, // swept circle
			const Vector<2, T> &, const Vector<2, T> &); // line
	template <typename T> std::pair<T, T>
		SweptCircleSegmentIntersectWeight(
			const Vector<2, T> &, const Vector<2, T> &, T radius, // swept circle
			const Vector<2, T> &, const Vector<2, T> &); // line
	template <typename T> std::pair<T, Vector<2, T>>
		SweptCircleSegmentIntersectWeightTangent(
			const Vector<2, T> &, const Vector<2, T> &, T radius, // swept circle
			const Vector<2, T> &, const Vector<2, T> &); // line

	// swept-sphere/swept-sphere intersection
	template <unsigned n, typename T> T
		SweptSphereIntersectWeight(
			const Vector<n, T> &, const Vector<n, T> &, T radius1,  // swept sphere
			const Vector<n, T> &, const Vector<n, T> &, T radius2); // swept sphere
}}

#	include "intersect.tpp"
#endif
