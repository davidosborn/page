/**
 * @copyright
 *
 * Copyright (c) 2006-2014 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the same place
 *    and form as other copyright, license, and disclaimer information.
 *
 * 3. Redistributions in binary form must also include an acknowledgement in the
 *    same place and form as other acknowledgements (such as the credits),
 *    similar in substance to the following:
 *
 *       Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#include <algorithm> // max, min
#include <cmath> // copysign, sqrt

#include "float.hpp" // Inf
#include "interp.hpp" // Lerp
#include "Plane.hpp"

namespace page { namespace math
{
	// closest point on line
	template <unsigned n, typename T> T ClosestPointOnLineWeight(const Vector<n, T> &a, const Vector<n, T> &b, const Vector<n, T> &p)
	{
		Vector<n, T> ab(b - a);
		return Dot(p - a, ab) / Dot(ab, ab);
	}
	template <unsigned n, typename T> Vector<n, T> ClosestPointOnLine(const Vector<n, T> &a, const Vector<n, T> &b, const Vector<n, T> &p)
	{
		return Lerp(a, b, ClosestPointOnLineWeight(a, b, p));
	}
	template <unsigned n, typename T> Vector<n, T> ClosestPointOnSegment(const Vector<n, T> &a, const Vector<n, T> &b, const Vector<n, T> &p)
	{
		T mu = ClosestPointOnLineWeight(a, b, p);
		if (mu <= 0) return a;
		if (mu >= 1) return b;
		return Lerp(a, b, mu);
	}

	// closest pont on plane
	template <unsigned n, typename T> T ClosestPointOnPlaneWeight(const Plane<n, T> &p, const Vector<n, T> &q)
	{
		// Real-Time Collision Detection, Christer Ericson, 2005
		assert(All(Near(p.no, Norm(p.no))));
		return Dot(p.no, q) - p.d;
	}
	template <unsigned n, typename T> Vector<n, T> ClosestPointOnPlane(const Plane<n, T> &p, const Vector<n, T> &q)
	{
		// Real-Time Collision Detection, Christer Ericson, 2005
		return q - ClosestPointOnPlaneWeight(p, q) * p.no;
	}

	// closest point between lines
	template <unsigned n, typename T> std::pair<T, T> ClosestPointSegmentWeight(const Vector<n, T> &a, const Vector<n, T> &b, const Vector<n, T> &c, const Vector<n, T> &d)
	{
		// Real-Time Collision Detection, Christer Ericson, 2005
		// FIXME: we should handle the degenerate cases
		// FIXME: this needs to be cleaned up
		Vector<n, T>
			ab(b - a),
			cd(d - c),
			ca(a - c);
		T
			e = Dot(ab, ab),
			f = Dot(cd, cd),
			g = Dot(ab, cd),
			h = Dot(ab, ca),
			i = Dot(cd, ca),
			denom = e * f - g * g,
			s = denom ? std::min<T>(std::max<T>((g * i - h * f) / denom, 0), 1) : 0,
			t = (g * s + i) / f;
		if (t < 0)
		{
			t = 0;
			s = std::min<T>(std::max<T>(-h / e, 0), 1);
		}
		else if (t > 1)
		{
			t = 1;
			s = std::min<T>(std::max<T>((g - h) / e, 0), 1);
		}
		return std::make_pair(s, t);
	}
	template <unsigned n, typename T> std::pair<Vector<n, T>, Vector<n, T>> ClosestPointSegment(const Vector<n, T> &a, const Vector<n, T> &b, const Vector<n, T> &c, const Vector<n, T> &d)
	{
		std::pair<T, T> mu(ClosestPointSegmentWeight(a, b, c, d));
		return std::make_pair(Lerp(a, b, mu.first), Lerp(c, d, mu.second));
	}

	// point/line distance
	template <unsigned n, typename T> T PointSegmentSqrDist(const Vector<n, T> &p, const Vector<n, T> &a, const Vector<n, T> &b)
	{
		// Real-Time Collision Detection, Christer Ericson, 2005
		Vector<n, T>
			ab(b - a),
			ap(p - a),
			bp(p - b);
		T mu = Dot(bp, ab);
		if (mu <= 0) return Dot(ap, ap);
		T denom = Dot(ab, ab);
		if (mu >= denom) return Dot(bp, bp);
		return Dot(ap, ap) - mu * mu / denom;
	}
	template <unsigned n, typename T> T PointSegmentDist(const Vector<n, T> &p, const Vector<n, T> &a, const Vector<n, T> &b)
	{
		return std::sqrt(PointSegmentSqrDist(p, a, b));
	}

	// line/line distance
	template <unsigned n, typename T> T SegmentSqrDist(const Vector<n, T> &a, const Vector<n, T> &b, const Vector<n, T> &c, const Vector<n, T> &d)
	{
		std::pair<Vector<n, T>, Vector<n, T>> p(ClosestPointSegment(a, b, c, d));
		Vector<n, T> v(p.second - p.first);
		return Dot(v, v);
	}
	template <unsigned n, typename T> T SegmentDist(const Vector<n, T> &a, const Vector<n, T> &b, const Vector<n, T> &c, const Vector<n, T> &d)
	{
		return std::sqrt(SegmentSqrDist(a, b, c, d));
	}

	// point/triangle intersection
	template <typename T> bool PointInTriangle(const Vector<2, T> &p, const Vector<2, T> &a, const Vector<2, T> &b, const Vector<2, T> &c)
	{
		return
			PerpDot(p - a, b - a) <= 0 &&
			PerpDot(p - b, c - b) <= 0 &&
			PerpDot(p - c, a - c) <= 0;
	}
	template <typename T> Vector<3, bool> PointInTriangleEdges(const Vector<2, T> &p, const Vector<2, T> &a, const Vector<2, T> &b, const Vector<2, T> &c)
	{
		return Vector<3, bool>(
			PerpDot(p - a, b - a) <= 0,
			PerpDot(p - b, c - b) <= 0,
			PerpDot(p - c, a - c) <= 0);
	}

	// line/line intersection
	template <typename T> T LineIntersectFirstWeight(const Vector<2, T> &a, const Vector<2, T> &b, const Vector<2, T> &c, const Vector<2, T> &d)
	{
		return
			PerpDot(d - c, a - c) /
			PerpDot(b - a, d - c);
	}
	template <typename T> T LineIntersectSecondWeight(const Vector<2, T> &a, const Vector<2, T> &b, const Vector<2, T> &c, const Vector<2, T> &d)
	{
		return
			PerpDot(b - a, a - c) /
			PerpDot(b - a, d - c);
	}
	template <typename T> std::pair<T, T> LineIntersectWeight(const Vector<2, T> &a, const Vector<2, T> &b, const Vector<2, T> &c, const Vector<2, T> &d)
	{
		Vector<2, T>
			ab(b - a),
			cd(d - c),
			ca(a - c);
		T badc = PerpDot(ab, cd);
		return std::make_pair(
			PerpDot(cd, ca) / badc,
			PerpDot(ab, ca) / badc);
	}
	template <typename T> Vector<2, T> LineIntersect(const Vector<2, T> &a, const Vector<2, T> &b, const Vector<2, T> &c, const Vector<2, T> &d)
	{
		return Lerp(a, b, LineIntersectFirstWeight(a, b, c, d));
	}
	template <typename T> std::pair<bool, Vector<2, T>> SegmentIntersect(const Vector<2, T> &a, const Vector<2, T> &b, const Vector<2, T> &c, const Vector<2, T> &d)
	{
		std::pair<T, T> mu(LineIntersectWeight(a, b, c, d));
		return std::make_pair(All(mu >= 0 && mu <= 1), Lerp(a, b, mu.first));
	}

	// line/circle intersection
	template <typename T> T LineCircleIntersectWeight(const Vector<2, T> &a, const Vector<2, T> &b, const Vector<2, T> &c, T r)
	{
		// http://www.idevgames.com/forum/showthread.php?t=5210#post51450
		Vector<2, T>
			ab(b - a),
			ac(c - a);
		T
			s = Dot(ab, ab),
			t = Dot(ac, ab) / s, // ClosestPointOnLineWeight(a, b, c)
			det = (r * r - Dot(ac, ac)) / s + t * t;
		if (det < 0) return Inf<T>();
		return t - std::sqrt(det);
	}
	template <typename T> std::pair<bool, T> SegmentCircleIntersect(const Vector<2, T> &a, const Vector<2, T> &b, const Vector<2, T> &c, T r)
	{
		T mu = ClosestPointOnLineWeight(a, b, c);
		if (mu < 0 || mu > 1 || Len(Lerp(a, b, mu) - c) > r)
			// HACK: for efficiency, weight is not provided here
			return std::make_pair(false, 0);
		return std::make_pair(true, LineCircleIntersectWeight(a, b, c, r));
	}

	// capsule/line intersection
	template <typename T> bool CapsuleSegmentIntersect(const Vector<2, T> &a, const Vector<2, T> &b, T r, const Vector<2, T> &c, const Vector<2, T> &d)
	{
		return SegmentSqrDist(a, b, c, d) <= r * r;
	}

	// swept-circle/line intersection
	template <typename T> T SweptCircleLineIntersectFirstWeight(const Vector<2, T> &a, const Vector<2, T> &b, T r, const Vector<2, T> &c, const Vector<2, T> &d)
	{
		// Real-Time Collision Detection, Christer Ericson, 2005
		Plane<2, T> p(Norm(Cross(d - c)), c);
		T
			dist = Dot(p.no, a) - p.d,
			denom = Dot(p.no, b - a);
		return (std::copysign(r, -denom) - dist) / denom;
	}
	template <typename T> std::pair<T, T> SweptCircleLineIntersectWeight(const Vector<2, T> &a, const Vector<2, T> &b, T r, const Vector<2, T> &c, const Vector<2, T> &d)
	{
		Vector<2, T> cd(d - c);
		Plane<2, T> p(Norm(Cross(cd)), c);
		T
			dist = Dot(p.no, a) - p.d,
			denom = Dot(p.no, b - a);
		r = std::copysign(r, -denom);
		T mu = (r - dist) / denom;
		Vector<2, T> q(Lerp(a, b, mu) - p.no * r);
		return std::make_pair(mu, Dot(q - c, cd) / Dot(cd, cd));
	}
	template <typename T> std::pair<T, T> SweptCircleSegmentIntersectWeight(const Vector<2, T> &a, const Vector<2, T> &b, T r, const Vector<2, T> &c, const Vector<2, T> &d)
	{
		// 1. move line toward circle origin by circle radius
		// 2. calculate swept-point/line intersection weight along line
		// 3. if weight in [0, 1] then SweptCircleLineIntersectWeight
		// 4. else treat end-point as circle and sweep vector as line
		Vector<2, T>
			no(Norm(Cross(d - c))), // line normal
			s(no * std::copysign(r, Dot(no, a - c))); // displacement
		T mu = LineIntersectSecondWeight(a, b, c + s, d + s);
		if (mu >= 0 && mu <= 1) return SweptCircleLineIntersectWeight(a, b, r, c, d);
		return std::make_pair(LineCircleIntersectWeight(a, b, mu > 0 ? d : c, r), mu > 0);
	}
	template <typename T> std::pair<T, Vector<2, T>> SweptCircleSegmentIntersectWeightTangent(const Vector<2, T> &a, const Vector<2, T> &b, T r, const Vector<2, T> &c, const Vector<2, T> &d)
	{
		std::pair<T, T> mu(SweptCircleSegmentIntersectWeight(a, b, r, c, d));
		return std::make_pair(mu.first, Norm(Cross(Lerp(a, b, mu.first) - Lerp(c, d, mu.second))));
	}

	// swept-sphere/swept-sphere intersection
	template <unsigned n, typename T> T SweptSphereIntersectWeight(const Vector<n, T> &a, const Vector<n, T> &b, T r1, const Vector<n, T> &c, const Vector<n, T> &d, T r2)
	{
		// FIXME: implement
	}
}}
