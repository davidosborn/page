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

#include <algorithm> // max, min, swap
#include <cassert>
#include <cmath> // acos, cos, sin, sqrt
#include <istream>
#include <ostream>

#include "Euler.hpp"
#include "float.hpp" // Near
#include "Matrix.hpp"
#include "Quat.hpp"
#include "Vector.hpp"

namespace page
{
	namespace math
	{
		// constructors
		template <typename T> Axan<T>::Axan(T x, T y, T z, T angle) :
			axis(x, y, z), angle(angle) {}
		template <typename T> Axan<T>::Axan(const Vector<3, T> &axis, T angle) :
			axis(axis), angle(angle) {}
		template <typename T> template <typename U> Axan<T>::Axan(const Axan<U> &other) :
			axis(other.axis), angle(other.angle) {}
		template <typename T> Axan<T>::Axan(const Euler<T> &e)
		{
			T
				cy = std::cos(e.yaw   / 2), sy = std::sin(e.yaw   / 2),
				cp = std::cos(e.pitch / 2), sp = std::sin(e.pitch / 2),
				cr = std::cos(e.roll  / 2), sr = std::sin(e.roll  / 2);
			axis = Vector<3, T>(
				cy * sp * cr + sy * cp * sr,
				sy * cp * cr - cy * sp * sr,
				cy * cp * sr - sy * sp * cr);
			T norm = Dot(axis, axis);
			if (norm < .001f) axis = NormVector<3, T>();
			else axis /= std::sqrt(norm);
			angle = std::acos(cy * cp * cr + sy * sp * sr) * 2;
		}
		template <typename T> Axan<T>::Axan(const Matrix<3, 3, T> &m) :
			axis(Norm(Vector<3, T>(m.zy - m.yz, m.xz - m.zx, m.yx - m.xy))),
			angle(std::acos(std::min<T>(std::max<T>((m.xx + m.yy + m.zz - 1) / 2, -1), 1))) {}
		template <typename T> Axan<T>::Axan(const Quat<T> &q)
		{
			assert(Near(q, Norm(q)));
			T len = std::sqrt(1 - q.w * q.w);
			axis = Near(len, 0) ? NormVector<3, T>() : Vector<3, T>(q) / len;
			angle = std::min<T>(std::max<T>(std::acos(q.w), -1), 1) * 2;
		}
		template <typename T> Axan<T>::Axan(const Vector<4, T> &v) :
			axis(v), angle(v.w) {}
		template <typename T> Axan<T>::Axan(const Vector<3, T> &v1, const Vector<3, T> &v2)
		{
			assert(All(Near(v1, Norm(v1))));
			assert(All(Near(v2, Norm(v2))));
			T t = Dot(v1, v2);
			axis =
				Near(t, -1) ? Swizzle(v1, 2, 1, 0) :
				Near(t, 1) ? NormVector<3, T>() : Norm(Cross(v1, v2));
			angle = std::acos(std::min<T>(std::max<T>(t, -1), 1));
		}

		// conversion
		template <typename T> template <typename U> Axan<T>::operator Axan<U>() const
		{
			return Axan<U>(axis, angle);
		}

		// relational operators
		template <typename T, typename U> bool operator ==(const Axan<T> &a1, const Axan<U> &a2)
		{
			return All(a1.axis == a2.axis) && a1.angle == a2.angle;
		}
		template <typename T, typename U> bool operator !=(const Axan<T> &a1, const Axan<U> &a2)
		{
			return Any(a1.axis != a2.axis) || a1.angle != a2.angle;
		}

		// other operations
		template <typename T, typename U> Axan<typename ArithmeticConversion<T, U>::Result> Between(const Axan<T> &a1, const Axan<U> &a2)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			return Axan<R>(a1 * NormVector<3, T>(), a2 * NormVector<3, U>());
		}
		template <typename T, typename U> bool Near(const Axan<T> &a1, const Axan<U> &a2)
		{
			return Near(a1.axis, a2.axis) && Near(a1.angle, a2.angle);
		}

		// vector transformation
		template <typename T, typename U> Vector<3, typename ArithmeticConversion<T, U>::Result> operator *(const Axan<T> &a, const Vector<3, U> &v)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			R
				s = std::sin(a.angle),
				c = std::cos(a.angle);
			return v + (1 - c) * Cross(a.axis, Cross(a.axis, v)) + s * Cross(a.axis, v);
		}

		// stream insertion/extraction
		template <typename T> std::ostream &operator <<(std::ostream &os, const Axan<T> &a)
		{
			return os << a.axis << ',' << a.angle;
		}
		template <typename T> std::istream &operator >>(std::istream &is, Axan<T> &a)
		{
			is >> a.axis;
			char c;
			if (is >> c && c != ',') is.setstate(std::ios_base::failbit);
			return is >> a.angle;
		}

		// specialized algorithms
		template <typename T> void swap(Axan<T> &a, Axan<T> &b)
		{
			using std::swap;
			swap(a.axis,  b.axis);
			swap(a.angle, b.angle);
		}
	}
}
