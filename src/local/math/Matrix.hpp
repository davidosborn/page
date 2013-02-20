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

#ifndef    page_local_math_Matrix_hpp
#   define page_local_math_Matrix_hpp

#	include <cstddef> // ptrdiff_t, size_t
#	include <iosfwd> // [io]stream
#	include <iterator> // reverse_iterator
#	include <type_traits> // enable_if

#	include "../util/iterator/proxy_iterator.hpp"
#	include "../util/iterator/step_iterator.hpp"
#	include "Aabb.hpp"
#	include "fwd.hpp" // Axan, defaultType, Euler, Matrix, Quat
#	include "Slice.hpp"
#	include "Vector.hpp"

namespace page
{
	namespace math
	{
		/**
		 * @class BasicMatrix
		 *
		 * The base class of @c Matrix, providing a standard implementation
		 * which all specializations share.
		 */
		template <unsigned nr, unsigned nc, typename T> struct BasicMatrix
		{
			// container traits
			typedef T value_type;
			typedef value_type &reference;
			typedef const value_type &const_reference;
			typedef value_type *iterator;
			typedef const value_type *const_iterator;
			typedef std::reverse_iterator<iterator> reverse_iterator;
			typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
			typedef std::ptrdiff_t difference_type;
			typedef std::size_t size_type;

			protected:
			// constructors
			BasicMatrix(T = 1, T = 0);
			BasicMatrix(const BasicMatrix &);
			template <unsigned mr, unsigned mc, typename U> explicit BasicMatrix(const BasicMatrix<mr, mc, U> &);
			explicit BasicMatrix(const Vector<nr < nc ? nr : nc, T> &);

			// assignment
			BasicMatrix &operator =(T);
			BasicMatrix &operator =(const BasicMatrix &);

			public:
			// iterators
			iterator begin();
			const_iterator begin() const;
			iterator end();
			const_iterator end() const;

			// reverse iterators
			reverse_iterator rbegin();
			const_reverse_iterator rbegin() const;
			reverse_iterator rend();
			const_reverse_iterator rend() const;

			// element access
			util::step_iterator<iterator, nr> operator [](size_type);
			util::step_iterator<const_iterator, nr> operator [](size_type) const;

			// slice types
			typedef Slice<nc, util::step_iterator<iterator, nr>> RowSlice;
			typedef Slice<nc, util::step_iterator<const_iterator, nr>> ConstRowSlice;
			typedef Slice<nr, iterator> ColSlice;
			typedef Slice<nr, const_iterator> ConstColSlice;
			typedef Slice<nr < nc ? nr : nc, util::step_iterator<iterator, nr + 1>> DiagSlice;
			typedef Slice<nr < nc ? nr : nc, util::step_iterator<const_iterator, nr + 1>> ConstDiagSlice;

			// slice access
			RowSlice Row(size_type);
			ConstRowSlice Row(size_type) const;
			ColSlice Col(size_type);
			ConstColSlice Col(size_type) const;
			DiagSlice Diag();
			ConstDiagSlice Diag() const;

			// slice iterator types
			typedef util::proxy_iterator<iterator, RowSlice (*)(value_type &), ConstRowSlice (*)(const value_type &)> RowIterator;
			typedef util::const_proxy_iterator<const_iterator, ConstRowSlice (*)(const value_type &)> ConstRowIterator;
			typedef util::proxy_iterator<util::step_iterator<iterator, nr>, ColSlice (*)(value_type &), ConstColSlice (*)(const value_type &)> ColIterator;
			typedef util::const_proxy_iterator<util::step_iterator<const_iterator, nr>, ConstColSlice (*)(const value_type &)> ConstColIterator;

			// slice iterators
			RowIterator BeginRows();
			ConstRowIterator BeginRows() const;
			RowIterator EndRows();
			ConstRowIterator EndRows() const;
			ColIterator BeginCols();
			ConstColIterator BeginCols() const;
			ColIterator EndCols();
			ConstColIterator EndCols() const;

			private:
			// slice factory functions
			static RowSlice MakeRowSlice(reference);
			static ConstRowSlice MakeConstRowSlice(const_reference);
			static ColSlice MakeColSlice(reference);
			static ConstColSlice MakeConstColSlice(const_reference);
		};

		/**
		 * @class Matrix
		 *
		 * A matrix class, with specializations for common configurations.
		 *
		 * @note The matrix is stored and manipulated in column major order, but
		 * is initialized and accessed in row major order.
		 */
		template <unsigned nr, unsigned nc, typename T> struct Matrix : BasicMatrix<nr, nc, T>
		{
			typedef BasicMatrix<nr, nc, T> Base;

			// constructors
			Matrix(T = 1, T = 0);
			template <unsigned mr, unsigned mc, typename U> explicit Matrix(const Matrix<mr, mc, U> &);
			explicit Matrix(const Vector<nr < nc ? nr : nc, T> &);

			// assignment
			Matrix &operator =(T);

			// conversion
			template <typename U> operator Matrix<nr, nc, U>() const;

			// HACK: GCC doesn't support using typename (bug #14258)
			typedef typename Base::iterator iterator;
			typedef typename Base::const_iterator const_iterator;

			using Base::begin;
			using Base::end;

			using Base::Row;
			using Base::Col;
			using Base::Diag;

			// HACK: GCC doesn't support using typename (bug #14258)
			typedef typename Base::RowIterator RowIterator;
			typedef typename Base::ConstRowIterator ConstRowIterator;
			typedef typename Base::ColIterator ColIterator;
			typedef typename Base::ConstColIterator ConstColIterator;

			using Base::BeginRows;
			using Base::EndRows;
			using Base::BeginCols;
			using Base::EndCols;

			// HACK: conceptually private
			// NOTE: extra storage for end pointer
			T _data[nr * (nc + 1) - 1];
		};
		template <unsigned n, typename T> struct Matrix<1, n, T> : BasicMatrix<1, n, T>
		{
			typedef BasicMatrix<1, n, T> Base;

			// FIXME: implement
		};
		template <unsigned n, typename T> struct Matrix<n, 1, T> : BasicMatrix<n, 1, T>
		{
			typedef BasicMatrix<n, 1, T> Base;

			// FIXME: implement
		};
		template <typename T> struct Matrix<2, 2, T> : BasicMatrix<2, 2, T>
		{
			typedef BasicMatrix<2, 2, T> Base;

			// constructors
			Matrix(T = 1, T = 0);
			Matrix(
				T, T,
				T, T);
			Matrix(const Vector<2, T> &, const Vector<2, T> &);
			template <unsigned mr, unsigned mc, typename U> explicit Matrix(const Matrix<mr, mc, U> &);
			explicit Matrix(const Vector<2, T> &);

			// assignment
			Matrix &operator =(T);

			// conversion
			template <typename U> operator Matrix<2, 2, U>() const;

			// HACK: GCC doesn't support using typename (bug #14258)
			typedef typename Base::iterator iterator;
			typedef typename Base::const_iterator const_iterator;

			using Base::begin;
			using Base::end;

			using Base::Row;
			using Base::Col;
			using Base::Diag;

			// HACK: GCC doesn't support using typename (bug #14258)
			typedef typename Base::RowIterator RowIterator;
			typedef typename Base::ConstRowIterator ConstRowIterator;
			typedef typename Base::ColIterator ColIterator;
			typedef typename Base::ConstColIterator ConstColIterator;

			using Base::BeginRows;
			using Base::EndRows;
			using Base::BeginCols;
			using Base::EndCols;

			union
			{
				__extension__ struct
				{
					T
						xx, yx,
						xy, yy;
				};
				// HACK: conceptually private
				// NOTE: extra storage for end pointer
				T _data[2 * (2 + 1) - 1];
			};
		};
		template <typename T> struct Matrix<3, 3, T> : BasicMatrix<3, 3, T>
		{
			typedef BasicMatrix<3, 3, T> Base;

			// constructors
			Matrix(T = 1, T = 0);
			Matrix(
				T, T, T,
				T, T, T,
				T, T, T);
			Matrix(const Vector<3, T> &, const Vector<3, T> &, const Vector<3, T> &);
			template <unsigned mr, unsigned mc, typename U> explicit Matrix(const Matrix<mr, mc, U> &);
			explicit Matrix(const Axan<T> &);
			explicit Matrix(const Euler<T> &);
			explicit Matrix(const Quat<T> &);
			explicit Matrix(const Vector<3, T> &);

			// assignment
			Matrix &operator =(T);

			// conversion
			template <typename U> operator Matrix<3, 3, U>() const;

			// HACK: GCC doesn't support using typename (bug #14258)
			typedef typename Base::iterator iterator;
			typedef typename Base::const_iterator const_iterator;

			using Base::begin;
			using Base::end;

			using Base::Row;
			using Base::Col;
			using Base::Diag;

			// HACK: GCC doesn't support using typename (bug #14258)
			typedef typename Base::RowIterator RowIterator;
			typedef typename Base::ConstRowIterator ConstRowIterator;
			typedef typename Base::ColIterator ColIterator;
			typedef typename Base::ConstColIterator ConstColIterator;

			using Base::BeginRows;
			using Base::EndRows;
			using Base::BeginCols;
			using Base::EndCols;

			union
			{
				__extension__ struct
				{
					T
						xx, yx, zx,
						xy, yy, zy,
						xz, yz, zz;
				};
				// HACK: conceptually private
				// NOTE: extra storage for end pointer
				T _data[3 * (3 + 1) - 1];
			};
		};
		template <typename T> struct Matrix<3, 4, T> : BasicMatrix<3, 4, T>
		{
			typedef BasicMatrix<3, 4, T> Base;

			// constructors
			Matrix(T = 1, T = 0);
			Matrix(
				T, T, T, T,
				T, T, T, T,
				T, T, T, T);
			Matrix(const Vector<4, T> &, const Vector<4, T> &, const Vector<4, T> &);
			template <unsigned mr, unsigned mc, typename U> explicit Matrix(const Matrix<mr, mc, U> &);
			explicit Matrix(const Axan<T> &);
			explicit Matrix(const Euler<T> &);
			explicit Matrix(const Quat<T> &);
			explicit Matrix(const Vector<3, T> &);

			// assignment
			Matrix &operator =(T);

			// conversion
			template <typename U> operator Matrix<3, 4, U>() const;

			// HACK: GCC doesn't support using typename (bug #14258)
			typedef typename Base::iterator iterator;
			typedef typename Base::const_iterator const_iterator;

			using Base::begin;
			using Base::end;

			using Base::Row;
			using Base::Col;
			using Base::Diag;

			// HACK: GCC doesn't support using typename (bug #14258)
			typedef typename Base::RowIterator RowIterator;
			typedef typename Base::ConstRowIterator ConstRowIterator;
			typedef typename Base::ColIterator ColIterator;
			typedef typename Base::ConstColIterator ConstColIterator;

			using Base::BeginRows;
			using Base::EndRows;
			using Base::BeginCols;
			using Base::EndCols;

			union
			{
				__extension__ struct
				{
					T
						xx, yx, zx,
						xy, yy, zy,
						xz, yz, zz,
						xw, yw, zw;
				};
				// HACK: conceptually private
				// NOTE: extra storage for end pointer
				T _data[3 * (4 + 1) - 1];
			};
		};
		template <typename T> struct Matrix<4, 4, T> : BasicMatrix<4, 4, T>
		{
			typedef BasicMatrix<4, 4, T> Base;

			// constructors
			Matrix(T = 1, T = 0);
			Matrix(
				T, T, T, T,
				T, T, T, T,
				T, T, T, T,
				T, T, T, T);
			Matrix(const Vector<4, T> &, const Vector<4, T> &, const Vector<4, T> &, const Vector<4, T> &);
			template <unsigned mr, unsigned mc, typename U> explicit Matrix(const Matrix<mr, mc, U> &);
			explicit Matrix(const Axan<T> &);
			explicit Matrix(const Euler<T> &);
			explicit Matrix(const Quat<T> &);
			explicit Matrix(const Vector<3, T> &);
			explicit Matrix(const Vector<4, T> &);

			// assignment
			Matrix &operator =(T);

			// conversion
			template <typename U> operator Matrix<4, 4, U>() const;

			// HACK: GCC doesn't support using typename (bug #14258)
			typedef typename Base::iterator iterator;
			typedef typename Base::const_iterator const_iterator;

			using Base::begin;
			using Base::end;

			using Base::Row;
			using Base::Col;
			using Base::Diag;

			// HACK: GCC doesn't support using typename (bug #14258)
			typedef typename Base::RowIterator RowIterator;
			typedef typename Base::ConstRowIterator ConstRowIterator;
			typedef typename Base::ColIterator ColIterator;
			typedef typename Base::ConstColIterator ConstColIterator;

			using Base::BeginRows;
			using Base::EndRows;
			using Base::BeginCols;
			using Base::EndCols;

			union
			{
				__extension__ struct
				{
					T
						xx, yx, zx, wx,
						xy, yy, zy, wy,
						xz, yz, zz, wz,
						xw, yw, zw, ww;
				};
				// HACK: conceptually private
				// NOTE: extra storage for end pointer
				T _data[4 * (4 + 1) - 1];
			};
		};

		// initialization
		template <unsigned nr, unsigned nc, typename T = defaultType> Matrix<nr, nc, T> ZeroMatrix();
		template <unsigned n, typename T> Matrix<n, n + 1, T> TranslationMatrix(const Vector<n, T> &);
		template <unsigned n, typename T> Matrix<n, n + 1, T> TranslationMatrix(T);
		template <typename T> Matrix<2, 2, T> RotationMatrix(T);
		template <typename T> Matrix<3, 3, T> RotationMatrix(const Axan<T> &);
		template <typename T> Matrix<3, 3, T> RotationMatrix(const Euler<T> &);
		template <typename T> Matrix<3, 3, T> RotationMatrix(const Quat<T> &);
		template <typename T> Matrix<3, 3, T> RotationMatrix(const Vector<3, T> &, const Vector<3, T> &);
		template <typename T> Matrix<3, 3, T> LookMatrix(const Vector<3, T> &, const Vector<3, T> &up = UpVector<3, T>());
		template <unsigned n, typename T> Matrix<n, n, T> ScaleMatrix(const Vector<n, T> &);
		template <unsigned n, typename T> Matrix<n, n, T> ScaleMatrix(T);
		template <typename T> Matrix<4, 4, T> PerspectiveMatrix(T fov, T aspect, T near, T far);
		template <typename T> Matrix<4, 4, T> InvPerspectiveMatrix(T fov, T aspect, T near, T far);
		template <typename T> Matrix<3, 4, T> OrthoMatrix(T left, T right, T bottom, T top, T near, T far);
		template <typename T> Matrix<3, 4, T> InvOrthoMatrix(T left, T right, T bottom, T top, T near, T far);

		// unary operators
		template <unsigned nr, unsigned nc, typename T> Matrix<nr, nc, T> operator +(const Matrix<nr, nc, T> &);
		template <unsigned nr, unsigned nc, typename T> Matrix<nr, nc, T> operator -(const Matrix<nr, nc, T> &);

		// arithmetic operators
		template <unsigned nr, unsigned nc, typename T, typename U, unsigned n> Matrix<nr, nc, typename ArithmeticConversion<T, U>::Result> operator *(const Matrix<nr, n, T> &, const Matrix<n, nc, U> &);
		template <typename T, typename U> Matrix<3, 4, typename ArithmeticConversion<T, U>::Result> operator *(const Matrix<3, 4, T> &, const Matrix<3, 4, U> &);
		template <unsigned nr, unsigned nc, typename T, typename U> Matrix<nr, nc, typename ArithmeticConversion<T, U>::Result> operator +(const Matrix<nr, nc, T> &, const Matrix<nr, nc, U> &);
		template <unsigned nr, unsigned nc, typename T, typename U> Matrix<nr, nc, typename ArithmeticConversion<T, U>::Result> operator -(const Matrix<nr, nc, T> &, const Matrix<nr, nc, U> &);

		// scalar arithmetic operators
		template <unsigned nr, unsigned nc, typename T, typename U> Matrix<nr, nc, typename ArithmeticConversion<T, U>::Result> operator *(const Matrix<nr, nc, T> &, U);
		template <unsigned nr, unsigned nc, typename T, typename U> Matrix<nr, nc, typename ArithmeticConversion<T, U>::Result> operator /(const Matrix<nr, nc, T> &, U);
		template <unsigned nr, unsigned nc, typename T, typename U> Matrix<nr, nc, typename ArithmeticConversion<T, U>::Result> operator +(const Matrix<nr, nc, T> &, U);
		template <unsigned nr, unsigned nc, typename T, typename U> Matrix<nr, nc, typename ArithmeticConversion<T, U>::Result> operator -(const Matrix<nr, nc, T> &, U);

		// reverse scalar arithmetic operators
		template <unsigned nr, unsigned nc, typename T, typename U> Matrix<nr, nc, typename ArithmeticConversion<T, U>::Result> operator *(T, const Matrix<nr, nc, U> &);
		template <unsigned nr, unsigned nc, typename T, typename U> Matrix<nr, nc, typename ArithmeticConversion<T, U>::Result> operator /(T, const Matrix<nr, nc, U> &);
		template <unsigned nr, unsigned nc, typename T, typename U> Matrix<nr, nc, typename ArithmeticConversion<T, U>::Result> operator +(T, const Matrix<nr, nc, U> &);
		template <unsigned nr, unsigned nc, typename T, typename U> Matrix<nr, nc, typename ArithmeticConversion<T, U>::Result> operator -(T, const Matrix<nr, nc, U> &);

		// relational operators
		template <unsigned nr, unsigned nc, typename T, typename U> bool operator ==(const Matrix<nr, nc, T> &, const Matrix<nr, nc, U> &);
		template <unsigned nr, unsigned nc, typename T, typename U> bool operator !=(const Matrix<nr, nc, T> &, const Matrix<nr, nc, U> &);

		// logical operators
		template <unsigned nr, unsigned nc> Matrix<nr, nc, bool> operator &&(const Matrix<nr, nc, bool> &, const Matrix<nr, nc, bool> &);
		template <unsigned nr, unsigned nc> Matrix<nr, nc, bool> operator ||(const Matrix<nr, nc, bool> &, const Matrix<nr, nc, bool> &);
		template <unsigned nr, unsigned nc> Matrix<nr, nc, bool> operator &&(const Matrix<nr, nc, bool> &, bool);
		template <unsigned nr, unsigned nc> Matrix<nr, nc, bool> operator ||(const Matrix<nr, nc, bool> &, bool);
		template <unsigned nr, unsigned nc> Matrix<nr, nc, bool> operator &&(bool, const Matrix<nr, nc, bool> &);
		template <unsigned nr, unsigned nc> Matrix<nr, nc, bool> operator ||(bool, const Matrix<nr, nc, bool> &);
		template <unsigned nr, unsigned nc> Matrix<nr, nc, bool> operator !(const Matrix<nr, nc, bool> &);

		// assignment operators
		template <unsigned nr, unsigned nc, typename T, typename U> Matrix<nr, nc, T> &operator *=(Matrix<nr, nc, T> &, const Matrix<nc, nc, U> &);
		template <typename T, typename U> Matrix<3, 4, T> &operator *=(Matrix<3, 4, T> &, const Matrix<3, 4, U> &);
		template <unsigned nr, unsigned nc, typename T, typename U> Matrix<nr, nc, T> &operator +=(Matrix<nr, nc, T> &, const Matrix<nr, nc, U> &);
		template <unsigned nr, unsigned nc, typename T, typename U> Matrix<nr, nc, T> &operator -=(Matrix<nr, nc, T> &, const Matrix<nr, nc, U> &);

		// scalar assignment operators
		template <unsigned nr, unsigned nc, typename T, typename U> Matrix<nr, nc, T> &operator *=(Matrix<nr, nc, T> &, U);
		template <unsigned nr, unsigned nc, typename T, typename U> Matrix<nr, nc, T> &operator /=(Matrix<nr, nc, T> &, U);
		template <unsigned nr, unsigned nc, typename T, typename U> Matrix<nr, nc, T> &operator +=(Matrix<nr, nc, T> &, U);
		template <unsigned nr, unsigned nc, typename T, typename U> Matrix<nr, nc, T> &operator -=(Matrix<nr, nc, T> &, U);

		// transformation extraction
		template <unsigned n, typename T> Vector<n, T> GetTranslation(const Matrix<n, n + 1, T> &);
		template <unsigned n, typename T> Vector<n - 1, T> GetTranslation(const Matrix<n, n, T> &);
		template <typename T> T GetRotation(const Matrix<2, 2, T> &);
		template <unsigned nr, unsigned nc, typename T> typename std::enable_if<nr == nc && (nr == 3 || nr == 4) || nr == 3 && nc == 4, Axan<T>>::type GetRotation(const Matrix<nr, nc, T> &);
		template <unsigned nr, unsigned nc, typename T> typename std::enable_if<nr == nc && (nr == 3 || nr == 4) || nr == 3 && nc == 4, Quat<T>>::type GetOrientation(const Matrix<nr, nc, T> &);
		template <unsigned nr, unsigned nc, typename T> Vector<nr < nc ? nr : nc, T> GetScale(const Matrix<nr, nc, T> &);
		template <typename T> Vector<3, T> GetScale(const Matrix<4, 4, T> &);

		// other operations
		template <unsigned nr, unsigned nc, typename T, typename U> Matrix<nr, nc, bool> Near(const Matrix<nr, nc, T> &, const Matrix<nr, nc, U> &);
		template <unsigned nr, unsigned nc, typename T> Matrix<nc, nr, T> Tpos(const Matrix<nr, nc, T> &);

		// AABB transformation
		template <typename T, typename U> Aabb<3, typename ArithmeticConversion<T, U>::Result> operator *(const Matrix<3, 3, T> &, const Aabb<3, U> &);
		template <typename T, typename U> Aabb<3, typename ArithmeticConversion<T, U>::Result> operator *(const Matrix<3, 4, T> &, const Aabb<3, U> &);

		// vector transformation
		template <unsigned nr, unsigned nc, typename T, typename U> Vector<nr, typename ArithmeticConversion<T, U>::Result> operator *(const Matrix<nr, nc, T> &, const Vector<nc, U> &);
		template <typename T, typename U> Vector<3, typename ArithmeticConversion<T, U>::Result> operator *(const Matrix<3, 4, T> &, const Vector<3, U> &);
		template <unsigned nr, unsigned nc, typename T, typename U> Vector<nc, typename ArithmeticConversion<T, U>::Result> operator *(const Vector<nr, T> &, const Matrix<nr, nc, U> &);
		template <unsigned n, typename T, typename U> Vector<n, T> &operator *=(Vector<n, T> &, const Matrix<n, n, U> &);

		// stream insertion/extraction
		template <unsigned nr, unsigned nc, typename T> std::ostream &operator <<(std::ostream &, const Matrix<nr, nc, T> &);
		template <unsigned nr, unsigned nc, typename T> std::istream &operator >>(std::istream &, Matrix<nr, nc, T> &);

		// specialized algorithms
		template <unsigned nr, unsigned nc, typename T> void swap(Matrix<nr, nc, T> &, Matrix<nr, nc, T> &);
	}
}

#	include "Matrix.tpp"
#endif
