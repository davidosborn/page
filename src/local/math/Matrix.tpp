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

#include <algorithm> // copy, fill, swap_ranges, transform
#include <cassert>
#include <cmath> // cos, sin, tan
#include <functional> // bind, divides, logical_{and,not,or}, minus, multiplies, negate, plus
#include <istream>
#include <ostream>

#include "../math/float.hpp" // Near
#include "../util/algorithm/stdext.hpp" // transform_n
#include "Axan.hpp"
#include "Euler.hpp"
#include "Quat.hpp"

namespace page
{
	namespace math
	{
		namespace detail
		{
			/**
			 * @class MatrixConversion
			 *
			 * The implementation of @c Convert.
			 *
			 * @see Convert
			 */
			template <unsigned, unsigned, typename, unsigned, unsigned, typename, bool, bool> struct MatrixConversion;
			template <unsigned nr, unsigned nc, typename T, unsigned mr, unsigned mc, typename U>
				struct MatrixConversion<nr, nc, T, mr, mc, U, true, true>
			{
				static Matrix<nr, nc, T> Apply(const BasicMatrix<mr, mc, U> &m)
				{
					Matrix<nr, nc, T> r;
					typename BasicMatrix<mr, mc, U>::ConstRowIterator mRow(m.BeginRows());
					for (typename BasicMatrix<nr, nc, T>::RowIterator rRow(r.BeginRows()); rRow != r.EndRows(); ++rRow, ++mRow)
						util::transform_n(mRow->begin(), nc, rRow->begin(), [](U t){ return static_cast<T>(t); });
					return r;
				}
			};
			template <unsigned nr, unsigned nc, typename T, unsigned mr, unsigned mc, typename U>
				struct MatrixConversion<nr, nc, T, mr, mc, U, true, false>
			{
				static Matrix<nr, nc, T> Apply(const BasicMatrix<mr, mc, U> &m)
				{
					Matrix<nr, nc, T> r;
					typename BasicMatrix<mr, mc, U>::ConstRowIterator mRow(m.BeginRows());
					for (typename BasicMatrix<nr, nc, T>::RowIterator rRow(r.BeginRows()); rRow != r.EndRows(); ++rRow, ++mRow)
						util::transform_n(mRow->begin(), mc, rRow->begin(), [](U t){ return static_cast<T>(t); });
					return r;
				}
			};
			template <unsigned nr, unsigned nc, typename T, unsigned mr, unsigned mc, typename U>
				struct MatrixConversion<nr, nc, T, mr, mc, U, false, true>
			{
				static Matrix<nr, nc, T> Apply(const BasicMatrix<mr, mc, U> &m)
				{
					Matrix<nr, nc, T> r;
					typename BasicMatrix<nr, nc, T>::RowIterator rRow(r.BeginRows());
					for (typename BasicMatrix<mr, mc, U>::ConstRowIterator mRow(m.BeginRows()); mRow != m.EndRows(); ++mRow, ++rRow)
						util::transform_n(mRow->begin(), nc, rRow->begin(), [](U t){ return static_cast<T>(t); });
					return r;
				}
			};
			template <unsigned nr, unsigned nc, typename T, unsigned mr, unsigned mc, typename U>
				struct MatrixConversion<nr, nc, T, mr, mc, U, false, false>
			{
				static Matrix<nr, nc, T> Apply(const BasicMatrix<mr, mc, U> &m)
				{
					Matrix<nr, nc, T> r;
					typename BasicMatrix<nr, nc, T>::RowIterator rRow(r.BeginRows());
					for (typename BasicMatrix<mr, mc, U>::ConstRowIterator mRow(m.BeginRows()); mRow != m.EndRows(); ++mRow, ++rRow)
						util::transform_n(mRow->begin(), mc, rRow->begin(), [](U t){ return static_cast<T>(t); });
					return r;
				}
			};
			/**
			 * @fn Convert
			 *
			 * Converts between matrices with different dimensions.
			 */
			template <unsigned nr, unsigned nc, typename T, unsigned mr, unsigned mc, typename U> Matrix<nr, nc, T> Convert(const BasicMatrix<mr, mc, U> &m)
				{ return MatrixConversion<nr, nc, T, mr, mc, U, nr <= mr, nc <= mc>::Apply(m); }
		}

		// basic matrix
		// constructors
		template <unsigned nr, unsigned nc, typename T> BasicMatrix<nr, nc, T>::BasicMatrix(T t1, T t0)
		{
			std::fill(begin(), end(), t0);
			Diag() = t1;
		}
		template <unsigned nr, unsigned nc, typename T> BasicMatrix<nr, nc, T>::BasicMatrix(const BasicMatrix &other)
		{
			std::copy(other.begin(), other.end(), begin());
		}
		template <unsigned nr, unsigned nc, typename T> template <unsigned mr, unsigned mc, typename U> BasicMatrix<nr, nc, T>::BasicMatrix(const BasicMatrix<mr, mc, U> &other)
		{
			*this = detail::Convert<nr, nc, T>(other);
		}
		template <unsigned nr, unsigned nc, typename T> BasicMatrix<nr, nc, T>::BasicMatrix(const Vector<nr < nc ? nr : nc, T> &v)
		{
			std::fill(begin(), end(), 0);
			Diag() = v;
		}

		// assignment
		template <unsigned nr, unsigned nc, typename T> BasicMatrix<nr, nc, T> &BasicMatrix<nr, nc, T>::operator =(T t)
		{
			std::fill(begin(), end(), 0);
			Diag() = t;
			return *this;
		}
		template <unsigned nr, unsigned nc, typename T> BasicMatrix<nr, nc, T> &BasicMatrix<nr, nc, T>::operator =(const BasicMatrix &other)
		{
			std::copy(other.begin(), other.end(), begin());
			return *this;
		}

		// iterators
		template <unsigned nr, unsigned nc, typename T> typename BasicMatrix<nr, nc, T>::iterator BasicMatrix<nr, nc, T>::begin()
			{ return static_cast<Matrix<nr, nc, T> &>(*this)._data; }
		template <unsigned nr, unsigned nc, typename T> typename BasicMatrix<nr, nc, T>::const_iterator BasicMatrix<nr, nc, T>::begin() const
			{ return static_cast<const Matrix<nr, nc, T> &>(*this)._data; }
		template <unsigned nr, unsigned nc, typename T> typename BasicMatrix<nr, nc, T>::iterator BasicMatrix<nr, nc, T>::end()
			{ return begin() + nr * nc; }
		template <unsigned nr, unsigned nc, typename T> typename BasicMatrix<nr, nc, T>::const_iterator BasicMatrix<nr, nc, T>::end() const
			{ return begin() + nr * nc; }

		// reverse iterators
		template <unsigned nr, unsigned nc, typename T> typename BasicMatrix<nr, nc, T>::reverse_iterator BasicMatrix<nr, nc, T>::rbegin()
			{ return reverse_iterator(end()); }
		template <unsigned nr, unsigned nc, typename T> typename BasicMatrix<nr, nc, T>::const_reverse_iterator BasicMatrix<nr, nc, T>::rbegin() const
			{ return const_reverse_iterator(end()); }
		template <unsigned nr, unsigned nc, typename T> typename BasicMatrix<nr, nc, T>::reverse_iterator BasicMatrix<nr, nc, T>::rend()
			{ return reverse_iterator(begin()); }
		template <unsigned nr, unsigned nc, typename T> typename BasicMatrix<nr, nc, T>::const_reverse_iterator BasicMatrix<nr, nc, T>::rend() const
			{ return const_reverse_iterator(begin()); }

		// element access
		template <unsigned nr, unsigned nc, typename T> util::step_iterator<typename BasicMatrix<nr, nc, T>::iterator, nr> BasicMatrix<nr, nc, T>::operator [](size_type i)
			{ return util::make_step_iterator<nr>(begin() + i); }
		template <unsigned nr, unsigned nc, typename T> util::step_iterator<typename BasicMatrix<nr, nc, T>::const_iterator, nr> BasicMatrix<nr, nc, T>::operator [](size_type i) const
			{ return util::make_step_iterator<nr>(begin() + i); }

		// slice access
		template <unsigned nr, unsigned nc, typename T> typename BasicMatrix<nr, nc, T>::RowSlice BasicMatrix<nr, nc, T>::Row(size_type i)
			{ return MakeRowSlice(begin()[i]); }
		template <unsigned nr, unsigned nc, typename T> typename BasicMatrix<nr, nc, T>::ConstRowSlice BasicMatrix<nr, nc, T>::Row(size_type i) const
			{ return MakeConstRowSlice(begin()[i]); }
		template <unsigned nr, unsigned nc, typename T> typename BasicMatrix<nr, nc, T>::ColSlice BasicMatrix<nr, nc, T>::Col(size_type i)
			{ return MakeColSlice(begin()[i * nr]); }
		template <unsigned nr, unsigned nc, typename T> typename BasicMatrix<nr, nc, T>::ConstColSlice BasicMatrix<nr, nc, T>::Col(size_type i) const
			{ return MakeConstColSlice(begin()[i * nr]); }
		template <unsigned nr, unsigned nc, typename T> typename BasicMatrix<nr, nc, T>::DiagSlice BasicMatrix<nr, nc, T>::Diag()
			{ return DiagSlice(util::make_step_iterator<nr + 1>(begin())); }
		template <unsigned nr, unsigned nc, typename T> typename BasicMatrix<nr, nc, T>::ConstDiagSlice BasicMatrix<nr, nc, T>::Diag() const
			{ return ConstDiagSlice(util::make_step_iterator<nr + 1>(begin())); }

		// slice iterators
		template <unsigned nr, unsigned nc, typename T> typename BasicMatrix<nr, nc, T>::RowIterator BasicMatrix<nr, nc, T>::BeginRows()
			{ return RowIterator(begin(), MakeRowSlice, MakeConstRowSlice); }
		template <unsigned nr, unsigned nc, typename T> typename BasicMatrix<nr, nc, T>::ConstRowIterator BasicMatrix<nr, nc, T>::BeginRows() const
			{ return ConstRowIterator(begin(), MakeConstRowSlice); }
		template <unsigned nr, unsigned nc, typename T> typename BasicMatrix<nr, nc, T>::RowIterator BasicMatrix<nr, nc, T>::EndRows()
			{ return RowIterator(begin() + nr, MakeRowSlice, MakeConstRowSlice); }
		template <unsigned nr, unsigned nc, typename T> typename BasicMatrix<nr, nc, T>::ConstRowIterator BasicMatrix<nr, nc, T>::EndRows() const
			{ return ConstRowIterator(begin() + nr, MakeConstRowSlice); }
		template <unsigned nr, unsigned nc, typename T> typename BasicMatrix<nr, nc, T>::ColIterator BasicMatrix<nr, nc, T>::BeginCols()
			{ return ColIterator(util::make_step_iterator<nr>(begin()), MakeColSlice, MakeConstColSlice); }
		template <unsigned nr, unsigned nc, typename T> typename BasicMatrix<nr, nc, T>::ConstColIterator BasicMatrix<nr, nc, T>::BeginCols() const
			{ return ConstColIterator(util::make_step_iterator<nr>(begin()), MakeConstColSlice); }
		template <unsigned nr, unsigned nc, typename T> typename BasicMatrix<nr, nc, T>::ColIterator BasicMatrix<nr, nc, T>::EndCols()
			{ return ColIterator(util::make_step_iterator<nr>(begin()) + nc, MakeColSlice, MakeConstColSlice); }
		template <unsigned nr, unsigned nc, typename T> typename BasicMatrix<nr, nc, T>::ConstColIterator BasicMatrix<nr, nc, T>::EndCols() const
			{ return ConstColIterator(util::make_step_iterator<nr>(begin()) + nc, MakeConstColSlice); }

		// slice factory functions
		template <unsigned nr, unsigned nc, typename T> typename BasicMatrix<nr, nc, T>::RowSlice BasicMatrix<nr, nc, T>::MakeRowSlice(reference c)
			{ return RowSlice(util::make_step_iterator<nr>(&c)); }
		template <unsigned nr, unsigned nc, typename T> typename BasicMatrix<nr, nc, T>::ConstRowSlice BasicMatrix<nr, nc, T>::MakeConstRowSlice(const_reference c)
			{ return ConstRowSlice(util::make_step_iterator<nr>(&c)); }
		template <unsigned nr, unsigned nc, typename T> typename BasicMatrix<nr, nc, T>::ColSlice BasicMatrix<nr, nc, T>::MakeColSlice(reference c)
			{ return ColSlice(&c); }
		template <unsigned nr, unsigned nc, typename T> typename BasicMatrix<nr, nc, T>::ConstColSlice BasicMatrix<nr, nc, T>::MakeConstColSlice(const_reference c)
			{ return ConstColSlice(&c); }

		// matrix
		// constructors
		template <unsigned nr, unsigned nc, typename T> Matrix<nr, nc, T>::Matrix(T t1, T t0) :
			Base(t1, t0) {}
		template <unsigned nr, unsigned nc, typename T> template <unsigned mr, unsigned mc, typename U> Matrix<nr, nc, T>::Matrix(const Matrix<mr, mc, U> &other) :
			Base(other) {}
		template <unsigned nr, unsigned nc, typename T> Matrix<nr, nc, T>::Matrix(const Vector<nr < nc ? nr : nc, T> &v) :
			Base(v) {}

		// assignment
		template <unsigned nr, unsigned nc, typename T> Matrix<nr, nc, T> &Matrix<nr, nc, T>::operator =(T t)
		{
			Base::operator =(t);
			return *this;
		}

		// conversion
		template <unsigned nr, unsigned nc, typename T> template <typename U> Matrix<nr, nc, T>::operator Matrix<nr, nc, U>() const
		{
			Matrix<nr, nc, U> r;
			std::copy(begin(), end(), r.begin());
			return r;
		}

		// 2x2 matrix
		// constructors
		template <typename T> Matrix<2, 2, T>::Matrix(T t1, T t0) :
			Base(t1, t0) {}
		template <typename T> Matrix<2, 2, T>::Matrix(
			T xx, T xy,
			T yx, T yy) :
			xx(xx), xy(xy),
			yx(yx), yy(yy) {}
		template <typename T> Matrix<2, 2, T>::Matrix(const Vector<2, T> &v1, const Vector<2, T> &v2)
		{
			Row(0) = v1;
			Row(1) = v2;
		}
		template <typename T> template <unsigned mr, unsigned mc, typename U> Matrix<2, 2, T>::Matrix(const Matrix<mr, mc, U> &other) :
			Base(other) {}
		template <typename T> Matrix<2, 2, T>::Matrix(const Vector<2, T> &v) :
			Base(v) {}

		// assignment
		template <typename T> Matrix<2, 2, T> &Matrix<2, 2, T>::operator =(T t)
		{
			Base::operator =(t);
			return *this;
		}

		// conversion
		template <typename T> template <typename U> Matrix<2, 2, T>::operator Matrix<2, 2, U>() const
		{
			Matrix<2, 2, U> r;
			std::copy(begin(), end(), r.begin());
			return r;
		}

		// 3x3 matrix
		// constructors
		template <typename T> Matrix<3, 3, T>::Matrix(T t1, T t0) :
			Base(t1, t0) {}
		template <typename T> Matrix<3, 3, T>::Matrix(
			T xx, T xy, T xz,
			T yx, T yy, T yz,
			T zx, T zy, T zz) :
			xx(xx), xy(xy), xz(xz),
			yx(yx), yy(yy), yz(yz),
			zx(zx), zy(zy), zz(zz) {}
		template <typename T> Matrix<3, 3, T>::Matrix(const Vector<3, T> &v1, const Vector<3, T> &v2, const Vector<3, T> &v3)
		{
			Row(0) = v1;
			Row(1) = v2;
			Row(2) = v3;
		}
		template <typename T> template <unsigned mr, unsigned mc, typename U> Matrix<3, 3, T>::Matrix(const Matrix<mr, mc, U> &other) :
			Base(other) {}
		template <typename T> Matrix<3, 3, T>::Matrix(const Axan<T> &a)
		{
			T
				c = std::cos(a.angle), s = std::sin(a.angle), t = 1 - c,
				sx = s * a.axis.x, sy = s * a.axis.y, sz = s * a.axis.z,
				tx = t * a.axis.x, ty = t * a.axis.y, tz = t * a.axis.z;
			xx = tx * a.axis.x + c;
			yx = tx * a.axis.y + sz;
			zx = tx * a.axis.z - sy;
			xy = ty * a.axis.x - sz;
			yy = ty * a.axis.y + c;
			zy = ty * a.axis.z + sx;
			xz = tz * a.axis.x + sy;
			yz = tz * a.axis.y - sx;
			zz = tz * a.axis.z + c;
		}
		template <typename T> Matrix<3, 3, T>::Matrix(const Euler<T> &e)
		{
			T
				cy = std::cos(e.yaw),   sy = std::sin(e.yaw),
				cp = std::cos(e.pitch), sp = std::sin(e.pitch),
				cr = std::cos(e.roll),  sr = std::sin(e.roll);
			xx = sy * sp * sr + cy * cr;
			yx = cp * sr;
			zx = cy * sp * sr - sy * cr;
			xy = sy * sp * cr - cy * sr;
			yy = cp * cr;
			zy = cy * sp * cr + sy * sr;
			xz = sy * cp;
			yz = -sp;
			zz = cy * cp;
		}
		template <typename T> Matrix<3, 3, T>::Matrix(const Quat<T> &q)
		{
			T
				qx2 = q.x * 2,   qy2 = q.y * 2,   qz2 = q.z * 2,
				qwx = q.w * qx2, qwy = q.w * qy2, qwz = q.w * qz2,
				qxx = q.x * qx2, qxy = q.x * qy2, qxz = q.x * qz2,
				qyy = q.y * qy2, qyz = q.y * qz2, qzz = q.z * qz2;
			xx = 1 - qyy - qzz;
			yx = qxy + qwz;
			zx = qxz - qwy;
			xy = qxy - qwz;
			yy = 1 - qxx - qzz;
			zy = qyz + qwx;
			xz = qxz + qwy;
			yz = qyz - qwx;
			zz = 1 - qxx - qyy;
		}
		template <typename T> Matrix<3, 3, T>::Matrix(const Vector<3, T> &v) :
			Base(v) {}

		// assignment
		template <typename T> Matrix<3, 3, T> &Matrix<3, 3, T>::operator =(T t)
		{
			Base::operator =(t);
			return *this;
		}

		// conversion
		template <typename T> template <typename U> Matrix<3, 3, T>::operator Matrix<3, 3, U>() const
		{
			Matrix<3, 3, U> r;
			std::copy(begin(), end(), r.begin());
			return r;
		}

		// 3x4 matrix
		// constructors
		template <typename T> Matrix<3, 4, T>::Matrix(T t1, T t0) :
			Base(t1, t0) {}
		template <typename T> Matrix<3, 4, T>::Matrix(
			T xx, T xy, T xz, T xw,
			T yx, T yy, T yz, T yw,
			T zx, T zy, T zz, T zw) :
			xx(xx), xy(xy), xz(xz), xw(xw),
			yx(yx), yy(yy), yz(yz), yw(yw),
			zx(zx), zy(zx), zz(zz), zw(zw) {}
		template <typename T> Matrix<3, 4, T>::Matrix(const Vector<4, T> &v1, const Vector<4, T> &v2, const Vector<4, T> &v3)
		{
			Row(0) = v1;
			Row(1) = v2;
			Row(2) = v3;
		}
		template <typename T> template <unsigned mr, unsigned mc, typename U> Matrix<3, 4, T>::Matrix(const Matrix<mr, mc, U> &other) :
			Base(other) {}
		template <typename T> Matrix<3, 4, T>::Matrix(const Axan<T> &a) :
			Base(Matrix<3, 3, T>(a)) {}
		template <typename T> Matrix<3, 4, T>::Matrix(const Euler<T> &e) :
			Base(Matrix<3, 3, T>(e)) {}
		template <typename T> Matrix<3, 4, T>::Matrix(const Quat<T> &q)	:
			Base(Matrix<3, 3, T>(q)) {}
		template <typename T> Matrix<3, 4, T>::Matrix(const Vector<3, T> &v) :
			Base(v) {}
		template <typename T> Matrix<3, 4, T> &Matrix<3, 4, T>::operator =(T t)
		{
			Base::operator =(t);
			return *this;
		}

		// conversion
		template <typename T> template <typename U> Matrix<3, 4, T>::operator Matrix<3, 4, U>() const
		{
			Matrix<3, 4, U> r;
			std::copy(begin(), end(), r.begin());
			return r;
		}

		// 4x4 matrix
		// construct/copy/assign
		template <typename T> Matrix<4, 4, T>::Matrix(T t1, T t0) :
			Base(t1, t0) {}
		template <typename T> Matrix<4, 4, T>::Matrix(
			T xx, T xy, T xz, T xw,
			T yx, T yy, T yz, T yw,
			T zx, T zy, T zz, T zw,
			T wx, T wy, T wz, T ww) :
			xx(xx), xy(xy), xz(xz), xw(xw),
			yx(yx), yy(yy), yz(yz), yw(yw),
			zx(zx), zy(zy), zz(zz), zw(zw),
			wx(wx), wy(wy), wz(wz), ww(ww) {}
		template <typename T> Matrix<4, 4, T>::Matrix(const Vector<4, T> &v1, const Vector<4, T> &v2, const Vector<4, T> &v3, const Vector<4, T> &v4)
		{
			Row(0) = v1;
			Row(1) = v2;
			Row(2) = v3;
			Row(3) = v4;
		}
		template <typename T> template <unsigned mr, unsigned mc, typename U> Matrix<4, 4, T>::Matrix(const Matrix<mr, mc, U> &other) :
			Base(other) {}
		template <typename T> Matrix<4, 4, T>::Matrix(const Axan<T> &a)	:
			Base(Matrix<3, 3, T>(a)) {}
		template <typename T> Matrix<4, 4, T>::Matrix(const Euler<T> &e) :
			Base(Matrix<3, 3, T>(e)) {}
		template <typename T> Matrix<4, 4, T>::Matrix(const Quat<T> &q) :
			Base(Matrix<3, 3, T>(q)) {}
		template <typename T> Matrix<4, 4, T>::Matrix(const Vector<3, T> &v) :
			Base(Vector<4, T>(v)) {}
		template <typename T> Matrix<4, 4, T>::Matrix(const Vector<4, T> &v) :
			Base(v) {}

		// assignment
		template <typename T> Matrix<4, 4, T> &Matrix<4, 4, T>::operator =(T t)
		{
			Base::operator =(t);
			return *this;
		}

		// conversion
		template <typename T> template <typename U> Matrix<4, 4, T>::operator Matrix<4, 4, U>() const
		{
			Matrix<4, 4, U> r;
			std::copy(begin(), end(), r.begin());
			return r;
		}

		// initialization
		template <unsigned nr, unsigned nc, typename T> Matrix<nr, nc, T> ZeroMatrix()
		{
			return Matrix<nr, nc, T>(0);
		}
		template <unsigned n, typename T> Matrix<n, n + 1, T> TranslationMatrix(const Vector<n, T> &v)
		{
			Matrix<n, n + 1, T> r;
			r.Col(n) = v;
			return r;
		}
		template <unsigned n, typename T> Matrix<n, n + 1, T> TranslationMatrix(T t)
		{
			Matrix<n, n + 1, T> r;
			r.Col(n) = t;
			return r;
		}
		template <typename T> Matrix<2, 2, T> RotationMatrix(T t)
		{
			return Matrix<2, 2, T>(
				std::cos(t), -std::sin(t),
				std::sin(t), std::cos(t));
		}
		template <typename T> Matrix<3, 3, T> RotationMatrix(const Axan<T> &a)
		{
			return Matrix<3, 3, T>(a);
		}
		template <typename T> Matrix<3, 3, T> RotationMatrix(const Euler<T> &e)
		{
			return Matrix<3, 3, T>(e);
		}
		template <typename T> Matrix<3, 3, T> RotationMatrix(const Quat<T> &q)
		{
			return Matrix<3, 3, T>(q);
		}
		template <typename T> Matrix<3, 3, T> RotationMatrix(const Vector<3, T> &v1, const Vector<3, T> &v2)
		{
			return Matrix<3, 3, T>(Axan<T>(v1, v2));
		}
		template <typename T> Matrix<3, 3, T> LookMatrix(const Vector<3, T> &v, const Vector<3, T> &up)
		{
			assert(All(Near(v, Norm(v))));
			assert(All(Near(up, Norm(up))));
			Vector<3> s(Norm(Cross(v, up)));
			return Tpos(Matrix<3, 3, T>(s, Norm(Cross(s, v)), -v));
		}
		template <unsigned n, typename T> Matrix<n, n, T> ScaleMatrix(const Vector<n, T> &v)
		{
			Matrix<n, n, T> r;
			r.Diag() = v;
			return r;
		}
		template <unsigned n, typename T> Matrix<n, n, T> ScaleMatrix(T t)
		{
			Matrix<n, n, T> r;
			r.Diag() = t;
			return r;
		}
		template <typename T> Matrix<4, 4, T> PerspectiveMatrix(T fov, T aspect, T near, T far)
		{
			// based on the definition of gluPerspective
			// http://www.opengl.org/sdk/docs/man/xhtml/gluPerspective.xml
			T d = 1 / std::tan(fov / 2);
			return Matrix<4, 4, T>(
				d / aspect, 0, 0, 0,
				0, d, 0, 0,
				0, 0, (near + far) / (near - far),
					near * far * 2 / (near - far),
				0, 0, -1, 0);
		}
		template <typename T> Matrix<4, 4, T> InvPerspectiveMatrix(T fov, T aspect, T near, T far)
		{
			// based on the definition of gluPerspective
			// http://www.opengl.org/sdk/docs/man/xhtml/gluPerspective.xml
			T d = std::tan(fov / 2);
			return Matrix<4, 4, T>(
				d * aspect, 0, 0, 0,
				0, d, 0, 0,
				0, 0, 0, -1,
				0, 0,
					(near - far) / (near * far * 2),
					(near + far) / (near * far * 2));
		}
		template <typename T> Matrix<3, 4, T> OrthoMatrix(T left, T right, T bottom, T top, T near, T far)
		{
			// based on the definition of glOrtho
			// http://www.opengl.org/sdk/docs/man/xhtml/glOrtho.xml
			return Matrix<3, 4, T>(
				2 / (right - left), 0, 0, (right + left) / (right - left),
				0, 2 / (top - bottom), 0, (top + bottom) / (top - bottom),
				0, 0, 2 / (near - far), (far + near) / (far - near));
		}
		template <typename T> Matrix<3, 4, T> InvOrthoMatrix(T left, T right, T bottom, T top, T near, T far)
		{
			// based on the definition of glOrtho
			// http://www.opengl.org/sdk/docs/man/xhtml/glOrtho.xml
			return Matrix<3, 4, T>(
				(right - left) / 2, 0, 0, (right + left) / 2,
				0, (top - bottom) / 2, 0, (top + bottom) / 2,
				0, 0, (near - far) / 2, -(near + far) / 2);
		}

		// unary operators
		template <unsigned nr, unsigned nc, typename T> Matrix<nr, nc, T> operator +(const Matrix<nr, nc, T> &m)
		{
			return m;
		}
		template <unsigned nr, unsigned nc, typename T> Matrix<nr, nc, T> operator -(const Matrix<nr, nc, T> &m)
		{
			Matrix<nr, nc, T> r;
			std::transform(m.begin(), m.end(), r.begin(), std::negate<T>());
			return r;
		}

		// arithmetic operators
		template <unsigned nr, unsigned nc, typename T, typename U, unsigned n> Matrix<nr, nc, typename ArithmeticConversion<T, U>::Result> operator *(const Matrix<nr, n, T> &m1, const Matrix<n, nc, U> &m2)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Matrix<nr, nc, R> r;
			typename Matrix<nr, nc, R>::iterator iter(r.begin());
			for (typename Matrix<n, nc, U>::ConstColIterator col(m2.BeginCols()); col != m2.EndCols(); ++col)
				for (typename Matrix<nr, n, T>::ConstRowIterator row(m1.BeginRows()); row != m1.EndRows(); ++row)
					*iter++ = Dot(Vector<n, T>(*row), Vector<n, U>(*col));
			return r;
		}
		template <typename T, typename U> Matrix<3, 4, typename ArithmeticConversion<T, U>::Result> operator *(const Matrix<3, 4, T> &m1, const Matrix<3, 4, U> &m2)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Matrix<3, 4, R> r;
			typename Matrix<3, 4, R>::iterator iter(r.begin());
			for (typename Matrix<3, 4, U>::ConstColIterator col(m2.BeginCols()); col != m2.EndCols(); ++col)
				for (typename Matrix<3, 4, T>::ConstRowIterator row(m1.BeginRows()); row != m1.EndRows(); ++row)
					*iter++ = Dot(Vector<3, T>(Vector<4, T>(*row)), Vector<3, U>(*col));
			r.Col(3) += m1.Col(3);
			return r;
		}
		template <unsigned nr, unsigned nc, typename T, typename U> Matrix<nr, nc, typename ArithmeticConversion<T, U>::Result> operator +(const Matrix<nr, nc, T> &m1, const Matrix<nr, nc, U> &m2)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Matrix<nr, nc, R> r;
			std::transform(m1.begin(), m1.end(), m2.begin(), r.begin(), std::plus<R>());
			return r;
		}
		template <unsigned nr, unsigned nc, typename T, typename U> Matrix<nr, nc, typename ArithmeticConversion<T, U>::Result> operator -(const Matrix<nr, nc, T> &m1, const Matrix<nr, nc, U> &m2)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Matrix<nr, nc, R> r;
			std::transform(m1.begin(), m1.end(), m2.begin(), r.begin(), std::minus<R>());
			return r;
		}

		// scalar arithmetic operators
		template <unsigned nr, unsigned nc, typename T, typename U> Matrix<nr, nc, typename ArithmeticConversion<T, U>::Result> operator *(const Matrix<nr, nc, T> &m, U t)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Matrix<nr, nc, R> r;
			std::transform(m.begin(), m.end(), r.begin(), std::bind(std::multiplies<R>(), std::placeholders::_1, t));
			return r;
		}
		template <unsigned nr, unsigned nc, typename T, typename U> Matrix<nr, nc, typename ArithmeticConversion<T, U>::Result> operator /(const Matrix<nr, nc, T> &m, U t)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Matrix<nr, nc, R> r;
			std::transform(m.begin(), m.end(), r.begin(), std::bind(std::divides<R>(), std::placeholders::_1, t));
			return r;
		}
		template <unsigned nr, unsigned nc, typename T, typename U> Matrix<nr, nc, typename ArithmeticConversion<T, U>::Result> operator +(const Matrix<nr, nc, T> &m, U t)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Matrix<nr, nc, R> r;
			std::transform(m.begin(), m.end(), r.begin(), std::bind(std::plus<R>(), std::placeholders::_1, t));
			return r;
		}
		template <unsigned nr, unsigned nc, typename T, typename U> Matrix<nr, nc, typename ArithmeticConversion<T, U>::Result> operator -(const Matrix<nr, nc, T> &m, U t)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Matrix<nr, nc, R> r;
			std::transform(m.begin(), m.end(), r.begin(), std::bind(std::minus<R>(), std::placeholders::_1, t));
			return r;
		}

		// reverse scalar arithmetic operators
		template <unsigned nr, unsigned nc, typename T, typename U> Matrix<nr, nc, typename ArithmeticConversion<T, U>::Result> operator *(T t, const Matrix<nr, nc, U> &m)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Matrix<nr, nc, R> r;
			std::transform(m.begin(), m.end(), r.begin(), std::bind(std::multiplies<R>(), t, std::placeholders::_1));
			return r;
		}
		template <unsigned nr, unsigned nc, typename T, typename U> Matrix<nr, nc, typename ArithmeticConversion<T, U>::Result> operator /(T t, const Matrix<nr, nc, U> &m)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Matrix<nr, nc, R> r;
			std::transform(m.begin(), m.end(), r.begin(), std::bind(std::divides<R>(), t, std::placeholders::_1));
			return r;
		}
		template <unsigned nr, unsigned nc, typename T, typename U> Matrix<nr, nc, typename ArithmeticConversion<T, U>::Result> operator +(T t, const Matrix<nr, nc, U> &m)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Matrix<nr, nc, R> r;
			std::transform(m.begin(), m.end(), r.begin(), std::bind(std::plus<R>(), t, std::placeholders::_1));
			return r;
		}
		template <unsigned nr, unsigned nc, typename T, typename U> Matrix<nr, nc, typename ArithmeticConversion<T, U>::Result> operator -(T t, const Matrix<nr, nc, U> &m)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Matrix<nr, nc, R> r;
			std::transform(m.begin(), m.end(), r.begin(), std::bind(std::minus<R>(), t, std::placeholders::_1));
			return r;
		}

		// relational operators
		template <unsigned nr, unsigned nc, typename T, typename U> bool operator ==(const Matrix<nr, nc, T> &m1, const Matrix<nr, nc, U> &m2)
		{
			return std::equal(m1.begin(), m1.end(), m2.begin());
		}
		template <unsigned nr, unsigned nc, typename T, typename U> bool operator !=(const Matrix<nr, nc, T> &m1, const Matrix<nr, nc, U> &m2)
		{
			return !std::equal(m1.begin(), m1.end(), m2.begin());
		}

		// logical operators
		template <unsigned nr, unsigned nc> Matrix<nr, nc, bool> operator &&(const Matrix<nr, nc, bool> &m1, const Matrix<nr, nc, bool> &m2)
		{
			Matrix<nr, nc, bool> r;
			std::transform(m1.begin(), m1.end(), m2.begin(), r.begin(), std::logical_and<bool>());
			return r;
		}
		template <unsigned nr, unsigned nc> Matrix<nr, nc, bool> operator ||(const Matrix<nr, nc, bool> &m1, const Matrix<nr, nc, bool> &m2)
		{
			Matrix<nr, nc, bool> r;
			std::transform(m1.begin(), m1.end(), m2.begin(), r.begin(), std::logical_or<bool>());
			return r;
		}
		template <unsigned nr, unsigned nc> Matrix<nr, nc, bool> operator &&(const Matrix<nr, nc, bool> &m, bool t)
		{
			Matrix<nr, nc, bool> r;
			std::transform(m.begin(), m.end(), r.begin(), std::bind(std::logical_and<bool>(), std::placeholders::_1, t));
			return r;
		}
		template <unsigned nr, unsigned nc> Matrix<nr, nc, bool> operator ||(const Matrix<nr, nc, bool> &m, bool t)
		{
			Matrix<nr, nc, bool> r;
			std::transform(m.begin(), m.end(), r.begin(), std::bind(std::logical_or<bool>(), std::placeholders::_1, t));
			return r;
		}
		template <unsigned nr, unsigned nc> Matrix<nr, nc, bool> operator &&(bool t, const Matrix<nr, nc, bool> &m)
		{
			Matrix<nr, nc, bool> r;
			std::transform(m.begin(), m.end(), r.begin(), std::bind(std::logical_and<bool>(), t, std::placeholders::_1));
			return r;
		}
		template <unsigned nr, unsigned nc> Matrix<nr, nc, bool> operator ||(bool t, const Matrix<nr, nc, bool> &m)
		{
			Matrix<nr, nc, bool> r;
			std::transform(m.begin(), m.end(), r.begin(), std::bind(std::logical_or<bool>(), t, std::placeholders::_1));
			return r;
		}
		template <unsigned nr, unsigned nc> Matrix<nr, nc, bool> operator !(const Matrix<nr, nc, bool> &m)
		{
			Matrix<nr, nc, bool> r;
			std::transform(m.begin(), m.end(), r.begin(), std::logical_not<bool>());
			return r;
		}

		// assignment operators
		template <unsigned nr, unsigned nc, typename T, typename U> Matrix<nr, nc, T> &operator *=(Matrix<nr, nc, T> &m1, const Matrix<nc, nc, U> &m2)
		{
			return m1 = m1 * m2;
		}
		template <typename T, typename U> Matrix<3, 4, T> &operator *=(Matrix<3, 4, T> &m1, const Matrix<3, 4, U> &m2)
		{
			return m1 = m1 * m2;
		}
		template <unsigned nr, unsigned nc, typename T, typename U> Matrix<nr, nc, T> &operator +=(Matrix<nr, nc, T> &m1, const Matrix<nr, nc, U> &m2)
		{
			std::transform(m1.begin(), m1.end(), m2.begin(), m1.begin(), std::plus<T>());
			return m1;
		}
		template <unsigned nr, unsigned nc, typename T, typename U> Matrix<nr, nc, T> &operator -=(Matrix<nr, nc, T> &m1, const Matrix<nr, nc, U> &m2)
		{
			std::transform(m1.begin(), m1.end(), m2.begin(), m1.begin(), std::minus<T>());
			return m1;
		}

		// scalar assignment operators
		template <unsigned nr, unsigned nc, typename T, typename U> Matrix<nr, nc, T> &operator *=(Matrix<nr, nc, T> &m, U t)
		{
			std::transform(m.begin(), m.end(), m.begin(), std::bind(std::multiplies<T>(), std::placeholders::_1, t));
			return m;
		}
		template <unsigned nr, unsigned nc, typename T, typename U> Matrix<nr, nc, T> &operator /=(Matrix<nr, nc, T> &m, U t)
		{
			std::transform(m.begin(), m.end(), m.begin(), std::bind(std::divides<T>(), std::placeholders::_1, t));
			return m;
		}
		template <unsigned nr, unsigned nc, typename T, typename U> Matrix<nr, nc, T> &operator +=(Matrix<nr, nc, T> &m, U t)
		{
			std::transform(m.begin(), m.end(), m.begin(), std::bind(std::plus<T>(), std::placeholders::_1, t));
			return m;
		}
		template <unsigned nr, unsigned nc, typename T, typename U> Matrix<nr, nc, T> &operator -=(Matrix<nr, nc, T> &m, U t)
		{
			std::transform(m.begin(), m.end(), m.begin(), std::bind(std::minus<T>(), std::placeholders::_1, t));
			return m;
		}

		// transformation extraction
		template <unsigned n, typename T> Vector<n, T> GetTranslation(const Matrix<n, n + 1, T> &m)
		{
			return m.Col(n);
		}
		template <unsigned n, typename T> Vector<n - 1, T> GetTranslation(const Matrix<n, n, T> &m)
		{
			return Vector<n - 1, T>(m.Col(n - 1));
		}
		template <typename T> T GetRotation(const Matrix<2, 2, T> &m)
		{
			// FIXME: implement
		}
		template <unsigned nr, unsigned nc, typename T> typename std::enable_if<nr == nc && (nr == 3 || nr == 4) || nr == 3 && nc == 4, Axan<T>>::type GetRotation(const Matrix<nr, nc, T> &m)
		{
			Matrix<3, 3, T> rm(m);
			Vector<3, T> scale(GetScale(rm));
			for (typename Matrix<3, 3, T>::ColIterator rmCol(rm.BeginCols()); rmCol != rm.EndCols(); ++rmCol)
				*rmCol /= scale;
			return Axan<T>(rm);
		}
		template <unsigned nr, unsigned nc, typename T> typename std::enable_if<nr == nc && (nr == 3 || nr == 4) || nr == 3 && nc == 4, Quat<T>>::type GetOrientation(const Matrix<nr, nc, T> &m)
		{
			Matrix<3, 3, T> rm(m);
			Vector<3, T> scale(GetScale(rm));
			for (typename Matrix<3, 3, T>::ColIterator rmCol(rm.BeginCols()); rmCol != rm.EndCols(); ++rmCol)
				*rmCol /= scale;
			return Quat<T>(rm);
		}
		template <unsigned nr, unsigned nc, typename T> Vector<nr < nc ? nr : nc, T> GetScale(const Matrix<nr, nc, T> &m)
		{
			typedef Vector<nr < nc ? nr : nc, T> SquareVector;
			SquareVector r;
			typename Matrix<nr, nc, T>::ConstRowIterator mRow(m.BeginRows());
			for (typename SquareVector::iterator iter(r.begin()); iter != r.end(); ++iter, ++mRow)
				*iter = Len(SquareVector(*mRow));
			return r;
		}
		template <typename T> Vector<3, T> GetScale(const Matrix<4, 4, T> &m)
		{
			Vector<3, T> r;
			typename Matrix<4, 4, T>::ConstRowIterator mRow(m.BeginRows());
			for (typename Vector<3, T>::iterator iter(r.begin()); iter != r.end(); ++iter, ++mRow)
				*iter = Len(Vector<3, T>(*mRow));
		}

		// other operations
		template <unsigned nr, unsigned nc, typename T, typename U> Matrix<nr, nc, bool> Near(const Matrix<nr, nc, T> &m1, const Matrix<nr, nc, U> &m2)
		{
			Matrix<nr, nc, bool> r;
			std::transform(m1.begin(), m1.end(), m2.begin(), r.begin(), [](T t1, U t2){ return Near(t1, t2); });
			return r;
		}
		template <unsigned nr, unsigned nc, typename T> Matrix<nc, nr, T> Tpos(const Matrix<nr, nc, T> &m)
		{
			Matrix<nc, nr, T> r;
			std::copy(m.BeginRows(), m.EndRows(), r.BeginCols());
			return r;
		}

		// AABB transformation
		template <typename T, typename U> Aabb<3, typename ArithmeticConversion<T, U>::Result> operator *(const Matrix<3, 3, T> &m, const Aabb<3, U> &a)
		{
			// Real-Time Collision Detection, Christer Ericson, 2005
			math::Aabb<3> r;
			for (int i = 0; i < 3; ++i)
			{
				math::Aabb<3> b(a * math::Vector<3>(m.Row(i)));
				r.min[i] = Sum(Min(b.min, b.max));
				r.max[i] = Sum(Max(b.min, b.max));
			}
			return r;
		}
		template <typename T, typename U> Aabb<3, typename ArithmeticConversion<T, U>::Result> operator *(const Matrix<3, 4, T> &m, const Aabb<3, U> &a)
		{
			// Real-Time Collision Detection, Christer Ericson, 2005
			return Matrix<3>(m) * a + GetTranslation(m);
		}

		// vector transformation
		template <unsigned nr, unsigned nc, typename T, typename U> Vector<nr, typename ArithmeticConversion<T, U>::Result> operator *(const Matrix<nr, nc, T> &m, const Vector<nc, U> &v)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Vector<nr, R> r;
			typename Vector<nr, R>::iterator iter(r.begin());
			for (typename Matrix<nr, nc, T>::ConstRowIterator row(m.BeginRows()); row != m.EndRows(); ++row)
				*iter++ = Dot(Vector<nc, T>(*row), v);
			return r;
		}
		template <typename T, typename U> Vector<3, typename ArithmeticConversion<T, U>::Result> operator *(const Matrix<3, 4, T> &m, const Vector<3, U> &v)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Vector<3, R> r;
			typename Vector<3, R>::iterator iter(r.begin());
			for (typename Matrix<3, 4, T>::ConstRowIterator row(m.BeginRows()); row != m.EndRows(); ++row)
				*iter++ = Dot(Vector<3, T>(Vector<4, T>(*row)), v) + row->w;
			return r;
		}
		template <unsigned nr, unsigned nc, typename T, typename U> Vector<nc, typename ArithmeticConversion<T, U>::Result> operator *(const Vector<nr, T> &v, const Matrix<nr, nc, U> &m)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Vector<nc, R> r;
			typename Vector<nc, R>::iterator iter(r.begin());
			for (typename Matrix<nr, nc, U>::ConstColIterator col(m.BeginCols()); col != m.EndCols(); ++col)
				*iter++ = Dot(v, Vector<nr, U>(*col));
			return r;
		}
		template <unsigned n, typename T, typename U> Vector<n, T> &operator *=(Vector<n, T> &v, const Matrix<n, n, U> &m)
		{
			return v = v * m;
		}

		// stream insertion/extraction
		template <unsigned nr, unsigned nc, typename T> std::ostream &operator <<(std::ostream &os, const Matrix<nr, nc, T> &m)
		{
			typename Matrix<nr, nc, T>::ConstRowIterator row(m.BeginRows());
			if (row != m.EndRows()) for (;;)
			{
				os << '[' << *row << ']';
				if (++row == m.EndRows()) break;
				os << ',';
			}
			return os;
		}
		template <unsigned nr, unsigned nc, typename T> std::istream &operator >>(std::istream &is, Matrix<nr, nc, T> &m)
		{
			typename Matrix<nr, nc, T>::RowIterator row(m.BeginRows());
			if (row != m.EndRows()) for (;;)
			{
				char c;
				if (is >> c && c != '[') is.setstate(std::ios_base::failbit);
				is >> *row;
				if (is >> c && c != ']') is.setstate(std::ios_base::failbit);
				if (++row == m.EndRows()) break;
				if (is >> c && c != ',') is.setstate(std::ios_base::failbit);
			}
			return is;
		}

		// specialized algorithms
		template <unsigned nr, unsigned nc, typename T> void swap(Matrix<nr, nc, T> &a, Matrix<nr, nc, T> &b)
		{
			std::swap_ranges(a.begin(), a.end(), b.begin());
		}
	}
}
