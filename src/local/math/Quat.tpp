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

#include <algorithm> // copy{,_n}, equal, fill_n, max, swap_ranges, transform
#include <cassert>
#include <cmath> // cos, sin, sqrt
#include <functional> // bind, divides, minus, multiplies, plus, negate
#include <numeric> // inner_product

#include "../math/float.hpp" // Near
#include "../util/algorithm/stdext.hpp" // find_not
#include "../util/functional/operator.hpp" // modulus
#include "../util/io/deserialize.hpp" // Deserialize
#include "../util/io/serialize.hpp" // Serialize
#include "../util/iterator/range.hpp"
#include "Axan.hpp"
#include "Euler.hpp"
#include "Matrix.hpp"

namespace page
{
	namespace math
	{
		// constructors
		template <typename T> Quat<T>::Quat()
		{
			*std::fill_n(begin(), 3, 0) = 1;
		}
		template <typename T> Quat<T>::Quat(T x, T y, T z) :
			x(x), y(y), z(z)
		{
			Vector<3, T> v(*this);
			w = std::sqrt(std::max<T>(1 - Dot(v, v), 0));
		}
		template <typename T> Quat<T>::Quat(T x, T y, T z, T w) :
			x(x), y(y), z(z), w(w) {}
		template <typename T> template <typename U> Quat<T>::Quat(const Quat<U> &other)
		{
			std::transform(other.begin(), other.end(), begin(), [](U t){ return static_cast<T>(t); });
		}
		template <typename T> Quat<T>::Quat(const Axan<T> &a)
		{
			assert(All(Near(a.axis, Norm(a.axis))));
			T t = a.angle / 2, s = std::sin(t);
			x = a.axis.x * s;
			y = a.axis.y * s;
			z = a.axis.z * s;
			w = std::cos(t);
		}
		template <typename T> Quat<T>::Quat(const Euler<T> &e)
		{
			T
				cy = std::cos(e.yaw   / 2), sy = std::sin(e.yaw   / 2),
				cp = std::cos(e.pitch / 2), sp = std::sin(e.pitch / 2),
				cr = std::cos(e.roll  / 2), sr = std::sin(e.roll  / 2);
			x = cy * sp * cr + sy * cp * sr;
			y = sy * cp * cr - cy * sp * sr;
			z = cy * cp * sr - sy * sp * cr;
			w = cy * cp * cr + sy * sp * sr;
		}
		template <typename T> Quat<T>::Quat(const Matrix<3, 3, T> &m)
		{
			// optimized algorithm by Christian in response to Martin Baker
			// http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/christian.htm
			// FIXME: this implementation doesn't appear to produce correct
			// results when facing below the horizon
			/*x = std::copysign(std::sqrt(std::max<T>(1 + m.xx - m.yy - m.zz, 0)) / 2, m.zy - m.yz);
			y = std::copysign(std::sqrt(std::max<T>(1 - m.xx + m.yy - m.zz, 0)) / 2, m.xz - m.zx);
			z = std::copysign(std::sqrt(std::max<T>(1 - m.xx - m.yy + m.zz, 0)) / 2, m.yx - m.xy);
			w = std::sqrt(std::max<T>(1 + m.xx + m.yy + m.zz, 0)) / 2;*/

			// reference implementation
			T trace = m.xx + m.yy + m.zz;
			if (trace > 0)
			{
				T s = .5f / std::sqrt(trace + 1);
				x = (m.zy - m.yz) * s;
				y = (m.xz - m.zx) * s;
				z = (m.yx - m.xy) * s;
				w = .25f / s;
			}
			else if (m.xx > m.yy && m.xx > m.zz)
			{
				T s = .5f / std::sqrt(m.xx - m.yy - m.zz + 1);
				x = .25f / s;
				y = (m.yx + m.xy) * s;
				z = (m.xz + m.zx) * s;
				w = (m.zy - m.yz) * s;
			}
			else if (m.yy > m.zz)
			{
				T s = .5f / std::sqrt(m.yy - m.zz - m.xx + 1);
				x = (m.yx + m.xy) * s;
				y = .25f / s;
				z = (m.zy + m.yz) * s;
				w = (m.xz - m.zx) * s;
			}
			else
			{
				T s = .5f / std::sqrt(m.zz - m.xx - m.yy + 1);
				x = (m.xz + m.zx) * s;
				y = (m.zy + m.yz) * s;
				z = .25f / s;
				w = (m.yx - m.xy) * s;
			}
			assert(Near(*this, Norm(*this)));
		}
		template <typename T> Quat<T>::Quat(const Vector<3, T> &v)
		{
			std::copy(v.begin(), v.end(), begin());
			w = std::sqrt(std::max<T>(1 - Dot(v, v), 0));
		}
		template <typename T> Quat<T>::Quat(const Vector<4, T> &v)
		{
			std::copy(v.begin(), v.end(), begin());
		}
		template <typename T> Quat<T>::Quat(const Vector<3, T> &v1, const Vector<3, T> &v2)
		{
			assert(All(Near(v1, Norm(v1))));
			assert(All(Near(v2, Norm(v2))));
			T t = Dot(v1, v2);
			if (Near(t, -1))
			{
				x = v1.z;
				y = v1.y;
				z = v1.x;
				w = 0;
			}
			else
			{
				T s = std::sqrt((t + 1) * 2);
				Vector<3> axis(Cross(v1, v2) / s);
				x = axis.x;
				y = axis.y;
				z = axis.z;
				w = s / 2;
			}
		}

		// conversion
		template <typename T> template <typename U> Quat<T>::operator Quat<U>() const
		{
			Quat<U> r;
			std::copy(begin(), end(), r.begin());
			return r;
		}
		template <typename T> Quat<T>::operator Vector<3, T>() const
		{
			Vector<3, T> r;
			std::copy_n(begin(), 3, r.begin());
			return r;
		}
		template <typename T> Quat<T>::operator Vector<4, T>() const
		{
			Vector<4, T> r;
			std::copy(begin(), end(), r.begin());
			return r;
		}

		// iterators
		template <typename T> typename Quat<T>::iterator Quat<T>::begin()
			{ return _data; }
		template <typename T> typename Quat<T>::const_iterator Quat<T>::begin() const
			{ return _data; }
		template <typename T> typename Quat<T>::iterator Quat<T>::end()
			{ return begin() + 4; }
		template <typename T> typename Quat<T>::const_iterator Quat<T>::end() const
			{ return begin() + 4; }

		// reverse iterators
		template <typename T> typename Quat<T>::reverse_iterator Quat<T>::rbegin()
			{ return reverse_iterator(end()); }
		template <typename T> typename Quat<T>::const_reverse_iterator Quat<T>::rbegin() const
			{ return const_reverse_iterator(end()); }
		template <typename T> typename Quat<T>::reverse_iterator Quat<T>::rend()
			{ return reverse_iterator(begin()); }
		template <typename T> typename Quat<T>::const_reverse_iterator Quat<T>::rend() const
			{ return const_reverse_iterator(begin()); }

		// element access
		template <typename T> typename Quat<T>::reference Quat<T>::operator [](size_type i)
			{ return begin()[i]; }
		template <typename T> typename Quat<T>::const_reference Quat<T>::operator [](size_type i) const
			{ return begin()[i]; }

		// zero equivalence
		template <typename T> Quat<T>::operator void *() const
		{
			return reinterpret_cast<void *>(util::find_not(begin(), end(), 0) != end());
		}

		// unary operators
		template <typename T> Quat<T> &operator ++(Quat<T> &q)
		{
			std::transform(q.begin(), q.end(), q.begin(), std::bind2nd(std::plus<T>(), 1));
			return q;
		}
		template <typename T> Quat<T> &operator --(Quat<T> &q)
		{
			std::transform(q.begin(), q.end(), q.begin(), std::bind2nd(std::minus<T>(), 1));
			return q;
		}
		template <typename T> Quat<T> operator ++(Quat<T> &q, int)
		{
			Quat<T> r;
			std::transform(q.begin(), q.end(), r.begin(), std::bind2nd(std::plus<T>(), 1));
			return r;
		}
		template <typename T> Quat<T> operator --(Quat<T> &q, int)
		{
			Quat<T> r;
			std::transform(q.begin(), q.end(), r.begin(), std::bind2nd(std::minus<T>(), 1));
			return r;
		}
		template <typename T> Quat<T> operator +(const Quat<T> &q)
		{
			return q;
		}
		template <typename T> Quat<T> operator -(const Quat<T> &q)
		{
			Quat<T> r;
			std::transform(q.begin(), q.end(), r.begin(), std::negate<T>());
			return r;
		}

		// arithmetic operators
		template <typename T, typename U> Quat<typename ArithmeticConversion<T, U>::Result> operator *(const Quat<T> &q1, const Quat<U> &q2)
		{
			return Quat<typename ArithmeticConversion<T, U>::Result>(
				q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y,
				q1.w * q2.y + q1.y * q2.w + q1.z * q2.x - q1.x * q2.z,
				q1.w * q2.z + q1.z * q2.w + q1.x * q2.y - q1.y * q2.x,
				q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z);
		}
		template <typename T, typename U> Quat<typename ArithmeticConversion<T, U>::Result> operator /(const Quat<T> &q1, const Quat<U> &q2)
		{
			return Inv(q2) * q1;
		}
		template <typename T, typename U> Quat<typename ArithmeticConversion<T, U>::Result> operator +(const Quat<T> &q1, const Quat<U> &q2)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Quat<R> r;
			std::transform(q1.begin(), q1.end(), q2.begin(), r.begin(), std::plus<R>());
			return r;
		}
		template <typename T, typename U> Quat<typename ArithmeticConversion<T, U>::Result> operator -(const Quat<T> &q1, const Quat<U> &q2)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Quat<R> r;
			std::transform(q1.begin(), q1.end(), q2.begin(), r.begin(), std::minus<R>());
			return r;
		}

		// scalar arithmetic operators
		template <typename T, typename U> Quat<typename ArithmeticConversion<T, U>::Result> operator *(const Quat<T> &q, U t)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Quat<R> r;
			std::transform(q.begin(), q.end(), r.begin(), std::bind(std::multiplies<R>(), std::placeholders::_1, t));
			return r;
		}
		template <typename T, typename U> Quat<typename ArithmeticConversion<T, U>::Result> operator /(const Quat<T> &q, U t)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Quat<R> r;
			std::transform(q.begin(), q.end(), r.begin(), std::bind(std::divides<R>(), std::placeholders::_1, t));
			return r;
		}
		template <typename T, typename U> Quat<typename ArithmeticConversion<T, U>::Result> operator %(const Quat<T> &q, U t)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Quat<R> r;
			std::transform(q.begin(), q.end(), r.begin(), std::bind(util::modulus<R>(), std::placeholders::_1, t));
			return r;
		}
		template <typename T, typename U> Quat<typename ArithmeticConversion<T, U>::Result> operator +(const Quat<T> &q, U t)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Quat<R> r;
			std::transform(q.begin(), q.end(), r.begin(), std::bind(std::plus<R>(), std::placeholders::_1, t));
			return r;
		}
		template <typename T, typename U> Quat<typename ArithmeticConversion<T, U>::Result> operator -(const Quat<T> &q, U t)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Quat<R> r;
			std::transform(q.begin(), q.end(), r.begin(), std::bind(std::minus<R>(), std::placeholders::_1, t));
			return r;
		}

		// reverse scalar arithmetic operators
		template <typename T, typename U> Quat<typename ArithmeticConversion<T, U>::Result> operator *(T t, const Quat<U> &q)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Quat<R> r;
			std::transform(q.begin(), q.end(), r.begin(), std::bind(std::multiplies<R>(), t, std::placeholders::_1));
			return r;
		}
		template <typename T, typename U> Quat<typename ArithmeticConversion<T, U>::Result> operator /(T t, const Quat<U> &q)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Quat<R> r;
			std::transform(q.begin(), q.end(), r.begin(), std::bind(std::divides<R>(), t, std::placeholders::_1));
			return r;
		}
		template <typename T, typename U> Quat<typename ArithmeticConversion<T, U>::Result> operator %(T t, const Quat<U> &q)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Quat<R> r;
			std::transform(q.begin(), q.end(), r.begin(), std::bind(util::modulus<R>(), t, std::placeholders::_1));
			return r;
		}
		template <typename T, typename U> Quat<typename ArithmeticConversion<T, U>::Result> operator +(T t, const Quat<U> &q)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Quat<R> r;
			std::transform(q.begin(), q.end(), r.begin(), std::bind(std::plus<R>(), t, std::placeholders::_1));
			return r;
		}
		template <typename T, typename U> Quat<typename ArithmeticConversion<T, U>::Result> operator -(T t, const Quat<U> &q)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Quat<R> r;
			std::transform(q.begin(), q.end(), r.begin(), std::bind(std::minus<R>(), t, std::placeholders::_1));
			return r;
		}

		// relational operators
		template <typename T, typename U> bool operator ==(const Quat<T> &q1, const Quat<U> &q2)
		{
			return std::equal(q1.begin(), q1.end(), q2.begin());
		}
		template <typename T, typename U> bool operator !=(const Quat<T> &q1, const Quat<U> &q2)
		{
			return !std::equal(q1.begin(), q1.end(), q2.begin());
		}

		// assignment operators
		template <typename T, typename U> Quat<T> &operator *=(Quat<T> &q1, const Quat<U> &q2)
		{
			return q1 = q1 * q2;
		}
		template <typename T, typename U> Quat<T> &operator /=(Quat<T> &q1, const Quat<U> &q2)
		{
			return q1 = q1 / q2;
		}
		template <typename T, typename U> Quat<T> &operator +=(Quat<T> &q1, const Quat<U> &q2)
		{
			std::transform(q1.begin(), q1.end(), q2.begin(), q1.begin(), std::plus<T>());
			return q1;
		}
		template <typename T, typename U> Quat<T> &operator -=(Quat<T> &q1, const Quat<U> &q2)
		{
			std::transform(q1.begin(), q1.end(), q2.begin(), q1.begin(), std::minus<T>());
			return q1;
		}

		// scalar assignment operators
		template <typename T, typename U> Quat<T> &operator *=(Quat<T> &q, U t)
		{
			std::transform(q.begin(), q.end(), q.begin(), std::bind(std::multiplies<T>(), std::placeholders::_1, t));
			return q;
		}
		template <typename T, typename U> Quat<T> &operator /=(Quat<T> &q, U t)
		{
			std::transform(q.begin(), q.end(), q.begin(), std::bind(std::divides<T>(), std::placeholders::_1, t));
			return q;
		}
		template <typename T, typename U> Quat<T> &operator %=(Quat<T> &q, U t)
		{
			std::transform(q.begin(), q.end(), q.begin(), std::bind(util::modulus<T>(), std::placeholders::_1, t));
			return q;
		}
		template <typename T, typename U> Quat<T> &operator +=(Quat<T> &q, U t)
		{
			std::transform(q.begin(), q.end(), q.begin(), std::bind(std::plus<T>(), std::placeholders::_1, t));
			return q;
		}
		template <typename T, typename U> Quat<T> &operator -=(Quat<T> &q, U t)
		{
			std::transform(q.begin(), q.end(), q.begin(), std::bind(std::minus<T>(), std::placeholders::_1, t));
			return q;
		}

		// other operations
		template <typename T, typename U> Quat<typename ArithmeticConversion<T, U>::Result> Between(const Quat<T> &q1, const Quat<U> &q2)
		{
			return q2 / q1;
		}
		template <typename T, typename U> typename ArithmeticConversion<T, U>::Result Dot(const Quat<T> &q1, const Quat<U> &q2)
		{
			return std::inner_product(q1.begin(), q1.end(), q2.begin(), typename ArithmeticConversion<T, U>::Result());
		}
		template <typename T> Quat<T> Inv(const Quat<T> &q)
		{
			return Quat<T>(-q.x, -q.y, -q.z, q.w);
		}
		template <typename T> T Len(const Quat<T> &q)
		{
			return std::sqrt(Dot(q, q));
		}
		template <typename T, typename U> bool Near(const Quat<T> &q1, const Quat<U> &q2)
		{
			return std::equal(q1.begin(), q1.end(), q2.begin(), [](T t1, U t2){ return Near(t1, t2); });
		}
		template <typename T> Quat<T> Norm(const Quat<T> &q)
		{
			assert(Len(q));
			return q / Len(q);
		}

		// axis-angle transformation
		template <typename T, typename U> Quat<typename ArithmeticConversion<T, U>::Result> operator *(const Quat<T> &q, const Axan<U> &a)
			{ return q * Quat<U>(a); }
		template <typename T, typename U> Quat<typename ArithmeticConversion<T, U>::Result> operator /(const Quat<T> &q, const Axan<U> &a)
			{ return q / Quat<U>(a); }
		template <typename T, typename U> Quat<T> &operator *=(const Quat<T> &q, const Axan<U> &a)
			{ return q *= Quat<U>(a); }
		template <typename T, typename U> Quat<T> &operator /=(const Quat<T> &q, const Axan<U> &a)
			{ return q /= Quat<U>(a); }

		// euler transformation
		template <typename T, typename U> Quat<typename ArithmeticConversion<T, U>::Result> operator *(const Quat<T> &q, const Euler<U> &e)
			{ return q * Quat<U>(e); }
		template <typename T, typename U> Quat<typename ArithmeticConversion<T, U>::Result> operator /(const Quat<T> &q, const Euler<U> &e)
			{ return q / Quat<U>(e); }
		template <typename T, typename U> Quat<T> &operator *=(const Quat<T> &q, const Euler<U> &e)
			{ return q *= Quat<U>(e); }
		template <typename T, typename U> Quat<T> &operator /=(const Quat<T> &q, const Euler<U> &e)
			{ return q /= Quat<U>(e); }

		// matrix transformation
		template <typename T, typename U> Quat<typename ArithmeticConversion<T, U>::Result> operator *(const Quat<T> &q, const Matrix<3, 3, U> &m)
			{ return q * Quat<U>(m); }
		template <typename T, typename U> Quat<typename ArithmeticConversion<T, U>::Result> operator /(const Quat<T> &q, const Matrix<3, 3, U> &m)
			{ return q / Quat<U>(m); }
		template <typename T, typename U> Quat<T> &operator *=(const Quat<T> &q, const Matrix<3, 3, U> &m)
			{ return q *= Quat<U>(m); }
		template <typename T, typename U> Quat<T> &operator /=(const Quat<T> &q, const Matrix<3, 3, U> &m)
			{ return q /= Quat<U>(m); }

		// vector transformation
		template <typename T, typename U> Vector<3, typename ArithmeticConversion<T, U>::Result> operator *(const Quat<T> &q, const Vector<3, U> &v)
		{
			return Vector<3>(q * Quat<T>(v) * Inv(q));
		}

		// stream insertion/extraction
		template <typename T> std::ostream &operator <<(std::ostream &os, const Quat<T> &q)
		{
			return util::Serialize(os, util::make_range(q), ',');
		}
		template <typename T> std::istream &operator >>(std::istream &is, Quat<T> &q)
		{
			return util::Deserialize(is, util::make_range(q), util::SequenceDeserializationFlags::none, ',');
		}

		// specialized algorithms
		template <typename T> void swap(Quat<T> &a, Quat<T> &b)
		{
			std::swap_ranges(a.begin(), a.end(), b.begin());
		}
	}
}
