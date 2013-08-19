#include <algorithm> // copy{,_n}, fill, swap_ranges, transform
#include <cassert>
#include <cmath> // cos, sin, tan
#include <functional> // bind, divides, logical_{and,not,or}, minus, multiplies, negate, plus

#include "../util/algorithm/stdext.hpp" // transform_n
#include "../util/io/separated_istream_iterator.hpp"
#include "../util/io/separated_ostream_iterator.hpp"
#include "Axan.hpp"
#include "Euler.hpp"
#include "float.hpp" // Near
#include "Quat.hpp"

namespace page { namespace math
{
	namespace detail
	{
		/**
		 * @class MatrixConversion
		 *
		 * The implementation of Convert().
		 */
		template <unsigned, unsigned, typename, unsigned, unsigned, typename, bool, bool>
			struct MatrixConversion;
		template <unsigned nr, unsigned nc, typename T, unsigned mr, unsigned mc, typename U>
			struct MatrixConversion<nr, nc, T, mr, mc, U, true, true>
		{
			static Matrix<nr, nc, T> Apply(const BasicMatrix<mr, mc, U> &m)
			{
				Matrix<nr, nc, T> r;
				auto mRow(m.Rows().begin());
				for (auto rRow(r.Rows().begin()); rRow != r.Rows().end(); ++rRow, ++mRow)
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
				auto mRow(m.Rows().begin());
				for (auto rRow(r.Rows().begin()); rRow != r.Rows().end(); ++rRow, ++mRow)
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
				auto rRow(r.Rows().begin());
				for (auto mRow(m.Rows().begin()); mRow != m.Rows().end(); ++mRow, ++rRow)
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
				auto rRow(r.Rows().begin());
				for (auto mRow(m.BeginRows()); mRow != m.EndRows(); ++mRow, ++rRow)
					util::transform_n(mRow->begin(), mc, rRow->begin(), [](U t){ return static_cast<T>(t); });
				return r;
			}
		};
		/**
		 * Converts between matrices with different dimensions.
		 */
		template <unsigned nr, unsigned nc, typename T, unsigned mr, unsigned mc, typename U>
			Matrix<nr, nc, T> Convert(const BasicMatrix<mr, mc, U> &m)
				{ return MatrixConversion<nr, nc, T, mr, mc, U, nr <= mr, nc <= mc>::Apply(m); }
	}

////////// BasicMatrix /////////////////////////////////////////////////////////

	/// assignment
	template <unsigned nr, unsigned nc, typename T>
		BasicMatrix<nr, nc, T> &BasicMatrix<nr, nc, T>::operator =(T diagonal)
	{
		std::fill(begin(), end(), 0);
		Diagonal() = diagonal;
		return *this;
	}

	template <unsigned nr, unsigned nc, typename T>
		BasicMatrix<nr, nc, T> &BasicMatrix<nr, nc, T>::operator =(const Vector<nr < nc ? nr : nc, T> &diagonal)
	{
		std::fill(begin(), end(), 0);
		Diagonal() = diagonal;
		return *this;
	}

	/// conversion
	template <unsigned nr, unsigned nc, typename T>
		template <typename U>
		BasicMatrix<nr, nc, T>::operator Matrix<nr, nc, U>() const
	{
		Matrix<nr, nc, U> r;
		std::copy(begin(), end(), r.begin());
		return r;
	}

	template <unsigned nr, unsigned nc, typename T>
		template <unsigned mr, unsigned mc, typename U>
		BasicMatrix<nr, nc, T>::operator Matrix<mr, mc, U>() const
	{
		return detail::Convert<mr, mc, U>(*this);
	}

	/// element access
	template <unsigned nr, unsigned nc, typename T>
		auto BasicMatrix<nr, nc, T>::operator [](size_type i) -> RowComponentIterator
			{ return RowComponentIterator(data() + i); }

	template <unsigned nr, unsigned nc, typename T>
		auto BasicMatrix<nr, nc, T>::operator [](size_type i) const -> ConstRowComponentIterator
			{ return ConstRowComponentIterator(data() + i); }

	template <unsigned nr, unsigned nc, typename T>
		T *BasicMatrix<nr, nc, T>::data() noexcept
			{ return static_cast<Matrix<nr, nc, T> &>(*this)._data; }

	template <unsigned nr, unsigned nc, typename T>
		const T *BasicMatrix<nr, nc, T>::data() const noexcept
			{ return static_cast<const Matrix<nr, nc, T> &>(*this)._data; }

	/// iterators
	template <unsigned nr, unsigned nc, typename T>
		auto BasicMatrix<nr, nc, T>::begin() -> iterator
			{ return data(); }

	template <unsigned nr, unsigned nc, typename T>
		auto BasicMatrix<nr, nc, T>::begin() const -> const_iterator
			{ return data(); }

	template <unsigned nr, unsigned nc, typename T>
		auto BasicMatrix<nr, nc, T>::end() -> iterator
			{ return begin() + nr * nc; }

	template <unsigned nr, unsigned nc, typename T>
		auto BasicMatrix<nr, nc, T>::end() const -> const_iterator
			{ return begin() + nr * nc; }

	template <unsigned nr, unsigned nc, typename T>
		auto BasicMatrix<nr, nc, T>::rbegin() -> reverse_iterator
			{ return reverse_iterator(end()); }

	template <unsigned nr, unsigned nc, typename T>
		auto BasicMatrix<nr, nc, T>::rbegin() const -> const_reverse_iterator
			{ return const_reverse_iterator(end()); }

	template <unsigned nr, unsigned nc, typename T>
		auto BasicMatrix<nr, nc, T>::rend() -> reverse_iterator
			{ return reverse_iterator(begin()); }

	template <unsigned nr, unsigned nc, typename T>
		auto BasicMatrix<nr, nc, T>::rend() const -> const_reverse_iterator
			{ return const_reverse_iterator(begin()); }

	/// slices
	template <unsigned nr, unsigned nc, typename T>
		auto BasicMatrix<nr, nc, T>::Row(size_type i) -> RowSlice
			{ return RowSlice(RowComponentIterator(begin() + i)); }

	template <unsigned nr, unsigned nc, typename T>
		auto BasicMatrix<nr, nc, T>::Row(size_type i) const -> ConstRowSlice
			{ return ConstRowSlice(ConstRowComponentIterator(begin() + i)); }

	template <unsigned nr, unsigned nc, typename T>
		auto BasicMatrix<nr, nc, T>::Column(size_type i) -> ColumnSlice
			{ return ColumnSlice(begin() + i * nr); }

	template <unsigned nr, unsigned nc, typename T>
		auto BasicMatrix<nr, nc, T>::Column(size_type i) const -> ConstColumnSlice
			{ return ConstColumnSlice(begin() + i * nr); }

	template <unsigned nr, unsigned nc, typename T>
		auto BasicMatrix<nr, nc, T>::Diagonal() -> DiagonalSlice
			{ return DiagonalSlice(DiagonalComponentIterator(begin())); }

	template <unsigned nr, unsigned nc, typename T>
		auto BasicMatrix<nr, nc, T>::Diagonal() const -> ConstDiagonalSlice
			{ return ConstDiagonalSlice(ConstDiagonalComponentIterator(begin())); }

	/// slice factory functions
	template <unsigned nr, unsigned nc, typename T>
		auto BasicMatrix<nr, nc, T>::MakeRowSlice::operator ()(const ColumnComponentIterator &iter) const -> RowSlice
			{ return RowSlice(RowComponentIterator(iter)); }

	template <unsigned nr, unsigned nc, typename T>
		auto BasicMatrix<nr, nc, T>::MakeConstRowSlice::operator ()(const ConstColumnComponentIterator &iter) const -> ConstRowSlice
			{ return ConstRowSlice(ConstRowComponentIterator(iter)); }

	template <unsigned nr, unsigned nc, typename T>
		auto BasicMatrix<nr, nc, T>::MakeColumnSlice::operator ()(const RowComponentIterator &iter) const -> ColumnSlice
			{ return ColumnSlice(iter.base()); }

	template <unsigned nr, unsigned nc, typename T>
		auto BasicMatrix<nr, nc, T>::MakeConstColumnSlice::operator ()(const ConstRowComponentIterator &iter) const -> ConstColumnSlice
			{ return ConstColumnSlice(iter.base()); }

	/// slice iterators
	template <unsigned nr, unsigned nc, typename T>
		auto BasicMatrix<nr, nc, T>::BeginRows() -> RowIterator
			{ return RowIterator(util::make_deferred_iterator(begin())); }

	template <unsigned nr, unsigned nc, typename T>
		auto BasicMatrix<nr, nc, T>::BeginRows() const -> ConstRowIterator
			{ return ConstRowIterator(util::make_deferred_iterator(begin())); }

	template <unsigned nr, unsigned nc, typename T>
		auto BasicMatrix<nr, nc, T>::EndRows() -> RowIterator
			{ return RowIterator(util::make_deferred_iterator(begin() + nr)); }

	template <unsigned nr, unsigned nc, typename T>
		auto BasicMatrix<nr, nc, T>::EndRows() const -> ConstRowIterator
			{ return ConstRowIterator(util::make_deferred_iterator(begin() + nr)); }

	template <unsigned nr, unsigned nc, typename T>
		auto BasicMatrix<nr, nc, T>::BeginColumns() -> ColumnIterator
			{ return ColumnIterator(util::make_deferred_iterator(RowComponentIterator(begin()))); }

	template <unsigned nr, unsigned nc, typename T>
		auto BasicMatrix<nr, nc, T>::BeginColumns() const -> ConstColumnIterator
			{ return ConstColumnIterator(util::make_deferred_iterator(ConstRowComponentIterator(begin()))); }

	template <unsigned nr, unsigned nc, typename T>
		auto BasicMatrix<nr, nc, T>::EndColumns() -> ColumnIterator
			{ return ColumnIterator(util::make_deferred_iterator(RowComponentIterator(begin()) + nc)); }

	template <unsigned nr, unsigned nc, typename T>
		auto BasicMatrix<nr, nc, T>::EndColumns() const -> ConstColumnIterator
			{ return ConstColumnIterator(util::make_deferred_iterator(ConstRowComponentIterator(begin()) + nc)); }

	/// slice ranges
	template <unsigned nr, unsigned nc, typename T>
		auto BasicMatrix<nr, nc, T>::Rows() -> boost::iterator_range<RowIterator>
			{ return boost::make_iterator_range(BeginRows(), EndRows()); }

	template <unsigned nr, unsigned nc, typename T>
		auto BasicMatrix<nr, nc, T>::Rows() const -> boost::iterator_range<ConstRowIterator>
			{ return boost::make_iterator_range(BeginRows(), EndRows()); }

	template <unsigned nr, unsigned nc, typename T>
		auto BasicMatrix<nr, nc, T>::Columns() -> boost::iterator_range<ColumnIterator>
			{ return boost::make_iterator_range(BeginColumns(), EndColumns()); }

	template <unsigned nr, unsigned nc, typename T>
		auto BasicMatrix<nr, nc, T>::Columns() const -> boost::iterator_range<ConstColumnIterator>
			{ return boost::make_iterator_range(BeginColumns(), EndColumns()); }

////////// Matrix //////////////////////////////////////////////////////////////

	/// constructors
	template <unsigned nr, unsigned nc, typename T>
		Matrix<nr, nc, T>::Matrix(T diagonal)
	{
		*this = diagonal;
	}

	template <unsigned nr, unsigned nc, typename T>
		Matrix<nr, nc, T>::Matrix(const Vector<nr < nc ? nr : nc, T> &diagonal)
	{
		*this = diagonal;
	}

////////// Matrix<2,2> /////////////////////////////////////////////////////////

	/// constructors
	template <typename T>
		Matrix<2, 2, T>::Matrix(T diagonal)
	{
		*this = diagonal;
	}

	template <typename T>
		Matrix<2, 2, T>::Matrix(const Vector<2, T> &diagonal)
	{
		*this = diagonal;
	}

	template <typename T>
		Matrix<2, 2, T>::Matrix(
			T xx, T xy,
			T yx, T yy) :
				xx(xx), xy(xy),
				yx(yx), yy(yy) {}

	template <typename T>
		Matrix<2, 2, T>::Matrix(
			const Vector<2, T> &v1,
			const Vector<2, T> &v2)
	{
		this->Row(0) = v1;
		this->Row(1) = v2;
	}

////////// Matrix<3,3> /////////////////////////////////////////////////////////

	/// constructors
	template <typename T>
		Matrix<3, 3, T>::Matrix(T diagonal)
	{
		*this = diagonal;
	}

	template <typename T>
		Matrix<3, 3, T>::Matrix(const Vector<3, T> &diagonal)
	{
		*this = diagonal;
	}

	template <typename T>
		Matrix<3, 3, T>::Matrix(
			T xx, T xy, T xz,
			T yx, T yy, T yz,
			T zx, T zy, T zz) :
				xx(xx), xy(xy), xz(xz),
				yx(yx), yy(yy), yz(yz),
				zx(zx), zy(zy), zz(zz) {}

	template <typename T>
		Matrix<3, 3, T>::Matrix(
			const Vector<3, T> &v1,
			const Vector<3, T> &v2,
			const Vector<3, T> &v3)
	{
		this->Row(0) = v1;
		this->Row(1) = v2;
		this->Row(2) = v3;
	}

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

////////// Matrix<3,4> /////////////////////////////////////////////////////////

	/// constructors
	template <typename T>
		Matrix<3, 4, T>::Matrix(T diagonal)
	{
		*this = diagonal;
	}

	template <typename T>
		Matrix<3, 4, T>::Matrix(const Vector<3, T> &diagonal)
	{
		*this = diagonal;
	}

	template <typename T>
		Matrix<3, 4, T>::Matrix(
			T xx, T xy, T xz, T xw,
			T yx, T yy, T yz, T yw,
			T zx, T zy, T zz, T zw) :
				xx(xx), xy(xy), xz(xz), xw(xw),
				yx(yx), yy(yy), yz(yz), yw(yw),
				zx(zx), zy(zx), zz(zz), zw(zw) {}

	template <typename T>
		Matrix<3, 4, T>::Matrix(
			const Vector<4, T> &v1,
			const Vector<4, T> &v2,
			const Vector<4, T> &v3)
	{
		this->Row(0) = v1;
		this->Row(1) = v2;
		this->Row(2) = v3;
	}

	template <typename T> Matrix<3, 4, T>::Matrix(const Axan<T> &a) :
		Base(Matrix<3, 3, T>(a)) {}

	template <typename T> Matrix<3, 4, T>::Matrix(const Euler<T> &e) :
		Base(Matrix<3, 3, T>(e)) {}

	template <typename T> Matrix<3, 4, T>::Matrix(const Quat<T> &q)	:
		Base(Matrix<3, 3, T>(q)) {}

////////// Matrix<4,4> /////////////////////////////////////////////////////////

	/// constructors
	template <typename T>
		Matrix<4, 4, T>::Matrix(T diagonal)
	{
		*this = diagonal;
	}

	template <typename T>
		Matrix<4, 4, T>::Matrix(const Vector<3, T> &v) :
			Matrix(Vector<4, T>(v)) {}

	template <typename T>
		Matrix<4, 4, T>::Matrix(const Vector<4, T> &diagonal)
	{
		*this = diagonal;
	}

	template <typename T>
		Matrix<4, 4, T>::Matrix(
			T xx, T xy, T xz, T xw,
			T yx, T yy, T yz, T yw,
			T zx, T zy, T zz, T zw,
			T wx, T wy, T wz, T ww) :
				xx(xx), xy(xy), xz(xz), xw(xw),
				yx(yx), yy(yy), yz(yz), yw(yw),
				zx(zx), zy(zy), zz(zz), zw(zw),
				wx(wx), wy(wy), wz(wz), ww(ww) {}

	template <typename T>
		Matrix<4, 4, T>::Matrix(
			const Vector<4, T> &v1,
			const Vector<4, T> &v2,
			const Vector<4, T> &v3,
			const Vector<4, T> &v4)
	{
		this->Row(0) = v1;
		this->Row(1) = v2;
		this->Row(2) = v3;
		this->Row(3) = v4;
	}

	template <typename T> Matrix<4, 4, T>::Matrix(const Axan<T> &a)	:
		Base(Matrix<3, 3, T>(a)) {}

	template <typename T> Matrix<4, 4, T>::Matrix(const Euler<T> &e) :
		Base(Matrix<3, 3, T>(e)) {}

	template <typename T> Matrix<4, 4, T>::Matrix(const Quat<T> &q) :
		Base(Matrix<3, 3, T>(q)) {}

////////// free functions //////////////////////////////////////////////////////

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
		auto s(Norm(Cross(v, up)));
		return Tpos(Matrix<3, 3, T>(s, Norm(Cross(s, v)), -v));
	}
	template <unsigned n, typename T> Matrix<n, n, T> ScaleMatrix(const Vector<n, T> &v)
	{
		Matrix<n, n, T> r;
		r.Diagonal() = v;
		return r;
	}
	template <unsigned n, typename T> Matrix<n, n, T> ScaleMatrix(T t)
	{
		Matrix<n, n, T> r;
		r.Diagonal() = t;
		return r;
	}
	template <typename T> Matrix<4, 4, T> PerspectiveMatrix(T fov, T aspect, T near, T far)
	{
		// based on the definition of gluPerspective
		// http://www.opengl.org/sdk/docs/man/xhtml/gluPerspective.xml
		auto d = 1 / std::tan(fov / 2);
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
		auto d = std::tan(fov / 2);
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
		for (typename Matrix<n, nc, U>::ConstColumnIterator col(m2.Columns().begin()); col != m2.Columns().end(); ++col)
			for (typename Matrix<nr, n, T>::ConstRowIterator row(m1.Rows().begin()); row != m1.Rows().end(); ++row)
				*iter++ = Dot(Vector<n, T>(*row), Vector<n, U>(*col));
		return r;
	}
	template <typename T, typename U> Matrix<3, 4, typename ArithmeticConversion<T, U>::Result> operator *(const Matrix<3, 4, T> &m1, const Matrix<3, 4, U> &m2)
	{
		typedef typename ArithmeticConversion<T, U>::Result R;
		Matrix<3, 4, R> r;
		typename Matrix<3, 4, R>::iterator iter(r.begin());
		for (typename Matrix<3, 4, U>::ConstColumnIterator col(m2.Columns().begin()); col != m2.Columns().end(); ++col)
			for (typename Matrix<3, 4, T>::ConstRowIterator row(m1.Rows().begin()); row != m1.Rows().end(); ++row)
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
		return Axan<T>(Norm(m));
	}
	template <unsigned nr, unsigned nc, typename T,
		typename std::enable_if<nr == nc && (nr == 3 || nr == 4) || nr == 3 && nc == 4>::type * = nullptr>
		Quat<T> GetOrientation(const Matrix<nr, nc, T> &m)
	{
		return Quat<T>(Norm(m));
	}

	template <unsigned nr, unsigned nc, typename T>
		Vector<nr < nc ? nr : nc, T> GetScale(const Matrix<nr, nc, T> &m)
	{
		Vector<nr < nc ? nr : nc, T> r;
		auto row(m.Rows().begin());
		for (auto &rr : r)
			rr = Len(decltype(r)(*row++));
		return r;
	}

	template <typename T>
		Vector<3, T> GetScale(const Matrix<4, 4, T> &m)
	{
		Vector<3, T> r;
		auto row(m.Rows().begin());
		for (auto &rr : r)
			rr = Len(decltype(r)(*row++));
		return r;
	}

	/// other operations
	template <unsigned nr, unsigned nc, typename T, typename U>
		Matrix<nr, nc, bool> Near(const Matrix<nr, nc, T> &a, const Matrix<nr, nc, U> &b)
	{
		Matrix<nr, nc, bool> r;
		std::transform(a.begin(), a.end(), b.begin(), r.begin(),
			[](T t1, U t2) { return Near(t1, t2); });
		return r;
	}

	template <unsigned nr, unsigned nc, typename T>
		Matrix<nc, nr, T> Tpos(const Matrix<nr, nc, T> &m)
	{
		Matrix<nc, nr, T> r;
		std::copy(m.Rows().begin(), m.Rows().end(), r.Columns().begin());
		return r;
	}

	template <unsigned nr, unsigned nc, typename T>
		Matrix<nr, nc, T> Norm(Matrix<nr, nc, T> m)
	{
		auto scale(GetScale(m));
		for (auto col(m.Columns().begin()); col != m.Columns().end(); ++col)
			*col /= scale;
		return m;
	}

	// AABB transformation
	template <typename T, typename U> Aabb<3, typename ArithmeticConversion<T, U>::Result> operator *(const Matrix<3, 3, T> &m, const Aabb<3, U> &a)
	{
		// Real-Time Collision Detection, Christer Ericson, 2005
		math::Aabb<3> r;
		for (int i = 0; i < 3; ++i)
		{
			math::Aabb<3> b(a * math::Vec3(m.Row(i)));
			r.min[i] = Sum(Min(b.min, b.max));
			r.max[i] = Sum(Max(b.min, b.max));
		}
		return r;
	}
	template <typename T, typename U> Aabb<3, typename ArithmeticConversion<T, U>::Result> operator *(const Matrix<3, 4, T> &m, const Aabb<3, U> &a)
	{
		// Real-Time Collision Detection, Christer Ericson, 2005
		return Mat3(m) * a + GetTranslation(m);
	}

	// vector transformation
	template <unsigned nr, unsigned nc, typename T, typename U> Vector<nr, typename ArithmeticConversion<T, U>::Result> operator *(const Matrix<nr, nc, T> &m, const Vector<nc, U> &v)
	{
		typedef typename ArithmeticConversion<T, U>::Result R;
		Vector<nr, R> r;
		typename Vector<nr, R>::iterator iter(r.begin());
		for (typename Matrix<nr, nc, T>::ConstRowIterator row(m.Rows().begin()); row != m.Rows().end(); ++row)
			*iter++ = Dot(Vector<nc, T>(*row), v);
		return r;
	}
	template <typename T, typename U> Vector<3, typename ArithmeticConversion<T, U>::Result> operator *(const Matrix<3, 4, T> &m, const Vector<3, U> &v)
	{
		typedef typename ArithmeticConversion<T, U>::Result R;
		Vector<3, R> r;
		typename Vector<3, R>::iterator iter(r.begin());
		for (typename Matrix<3, 4, T>::ConstRowIterator row(m.Rows().begin()); row != m.Rows().end(); ++row)
			*iter++ = Dot(Vector<3, T>(Vector<4, T>(*row)), v) + row->w;
		return r;
	}
	template <unsigned nr, unsigned nc, typename T, typename U> Vector<nc, typename ArithmeticConversion<T, U>::Result> operator *(const Vector<nr, T> &v, const Matrix<nr, nc, U> &m)
	{
		typedef typename ArithmeticConversion<T, U>::Result R;
		Vector<nc, R> r;
		typename Vector<nc, R>::iterator iter(r.begin());
		for (typename Matrix<nr, nc, U>::ConstColumnIterator col(m.Columns().begin()); col != m.Columns().end(); ++col)
			*iter++ = Dot(v, Vector<nr, U>(*col));
		return r;
	}
	template <unsigned n, typename T, typename U> Vector<n, T> &operator *=(Vector<n, T> &v, const Matrix<n, n, U> &m)
	{
		return v = v * m;
	}

	// stream insertion/extraction
	template <typename Char, typename CharTraits, unsigned nr, unsigned nc, typename T>
		std::basic_ostream<Char, CharTraits> &operator <<(std::basic_ostream<Char, CharTraits> &os, const Matrix<nr, nc, T> &m)
	{
		os << CharTraits::to_char_type('(');
		std::copy(m.BeginRows(), m.EndRows(),
			util::separated_ostream_iterator<typename Matrix<nr, nc, T>::RowSlice, Char, CharTraits>(os, CharTraits::to_char_type(',')));
		os << CharTraits::to_char_type(')');
		return os;
	}
	template <typename Char, typename CharTraits, unsigned nr, unsigned nc, typename T>
		std::basic_istream<Char, CharTraits> &operator >>(std::basic_istream<Char, CharTraits> &is, Matrix<nr, nc, T> &m)
	{
		// read opening bracket
		Char ch;
		is >> ch;
		if (!CharTraits::eq(ch, CharTraits::to_char_type('(')))
			is.setstate(std::ios_base::failbit);

		// read planes
		std::copy_n(
			util::separated_istream_iterator<Vector<nc, T>, Char, CharTraits>(is, CharTraits::to_char_type(',')),
			nr, m.BeginRows());

		// read closing bracket
		is >> ch;
		if (!CharTraits::eq(ch, CharTraits::to_char_type(')')))
			is.setstate(std::ios_base::failbit);

		return is;
	}

	// standard-library compatibility
	template <unsigned nr, unsigned nc, typename T> void swap(Matrix<nr, nc, T> &a, Matrix<nr, nc, T> &b)
	{
		std::swap_ranges(a.begin(), a.end(), b.begin());
	}
}}
