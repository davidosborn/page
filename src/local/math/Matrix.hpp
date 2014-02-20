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

#ifndef    page_math_Matrix_hpp
#   define page_math_Matrix_hpp

#	include <cstddef> // ptrdiff_t, size_t
#	include <iosfwd> // basic_[io]stream
#	include <iterator> // reverse_iterator

#	include <boost/range/iterator_range.hpp>

#	include "../util/iterator/deferred_iterator.hpp"
#	include "../util/iterator/proxy_iterator.hpp"
#	include "../util/iterator/step_iterator.hpp"
#	include "../util/type_traits/sfinae.hpp" // ENABLE_IF
#	include "Aabb.hpp"
#	include "fwd.hpp" // Axan, DefaultType, Euler, Matrix, Quat
#	include "Slice.hpp"
#	include "Vector.hpp"

namespace page { namespace math
{

////////// BasicMatrix /////////////////////////////////////////////////////////

	/**
	 * The base class of @c Matrix which implements the parts that are common to
	 * all specializations.
	 */
	template <unsigned nr, unsigned nc, typename T>
		class BasicMatrix
	{
		/*-----------------+
		| container traits |
		+-----------------*/

		public:
		using value_type             = T;
		using reference              = value_type &;
		using const_reference        = const value_type &;
		using iterator               = value_type *;
		using const_iterator         = const value_type *;
		using reverse_iterator       = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;
		using difference_type        = std::ptrdiff_t;
		using size_type              = std::size_t;

		/*-----------+
		| assignment |
		+-----------*/

		protected:
		/**
		 * Turns the matrix into a scalar matrix with the specified diagonal
		 * value.
		 */
		BasicMatrix &operator =(T diagonal);

		/**
		 * Turns the matrix into a diagonal matrix with the specified diagonal
		 * vector.
		 */
		BasicMatrix &operator =(const Vector<nr < nc ? nr : nc, T> &diagonal);

		/*-----------+
		| conversion |
		+-----------*/

		public:
		/**
		 * Permits implicit conversion of the component type.
		 */
		template <typename U>
			operator Matrix<nr, nc, U>() const;

		/**
		 * Permits explicit conversion of the dimension and component type.
		 */
		template <unsigned mr, unsigned mc, typename U>
			explicit operator Matrix<mr, mc, U>() const;

		/*-------------------------+
		| component-iterator types |
		+-------------------------*/

		/**
		 * An iterator over the components of a row.
		 */
		using RowComponentIterator = util::step_iterator<iterator, nr>;

		/**
		 * An iterator over the components of a row.
		 */
		using ConstRowComponentIterator = util::step_iterator<const_iterator, nr>;

		/**
		 * An iterator over the components of a column.
		 */
		using ColumnComponentIterator = iterator;

		/**
		 * An iterator over the components of a column.
		 */
		using ConstColumnComponentIterator = const_iterator;

		/**
		 * An iterator over the components of the diagonal vector.
		 */
		using DiagonalComponentIterator = util::step_iterator<iterator, nr + 1>;

		/**
		 * An iterator over the components of the diagonal vector.
		 */
		using ConstDiagonalComponentIterator = util::step_iterator<const_iterator, nr + 1>;

		/*---------------+
		| element access |
		+---------------*/

		/**
		 * @return An iterator over the components of the row at @index,
		 * starting at the first component of the row.
		 */
		RowComponentIterator operator [](size_type index);

		/**
		 * @return An iterator over the components of the row at @index,
		 * starting at the first component of the row.
		 */
		ConstRowComponentIterator operator [](size_type index) const;

		/**
		 * @return A pointer to the matrix's first component.  The components
		 * are stored contiguously.
		 */
		T *data() noexcept;

		/**
		 * @return A pointer to the matrix's first component.  The components
		 * are stored contiguously.
		 */
		const T *data() const noexcept;

		/*----------+
		| iterators |
		+----------*/

		/**
		 * @return An iterator over the matrix's components, which points to the
		 * first component.
		 */
		iterator begin();

		/**
		 * @return An iterator over the matrix's components, which points to the
		 * first component.
		 */
		const_iterator begin() const;

		/**
		 * @return An iterator over the matrix's components, which points to one
		 * past the last component.
		 */
		iterator end();

		/**
		 * @return An iterator over the matrix's components, which points to one
		 * past the last component.
		 */
		const_iterator end() const;

		/**
		 * @return A reverse iterator over the matrix's components, which points
		 * to the last component.
		 */
		reverse_iterator rbegin();

		/**
		 * @return A reverse iterator over the matrix's components, which points
		 * to the last component.
		 */
		const_reverse_iterator rbegin() const;

		/**
		 * @return A reverse iterator over the matrix's components, which points
		 * to one past the first component.
		 */
		reverse_iterator rend();

		/**
		 * @return A reverse iterator over the matrix's components, which points
		 * to one past the first component.
		 */
		const_reverse_iterator rend() const;

		/*------------+
		| slice types |
		+------------*/

		using RowSlice           = Slice<nc, RowComponentIterator>;
		using ConstRowSlice      = Slice<nc, ConstRowComponentIterator>;
		using ColumnSlice        = Slice<nr, ColumnComponentIterator>;
		using ConstColumnSlice   = Slice<nr, ConstColumnComponentIterator>;
		using DiagonalSlice      = Slice<nr < nc ? nr : nc, DiagonalComponentIterator>;
		using ConstDiagonalSlice = Slice<nr < nc ? nr : nc, ConstDiagonalComponentIterator>;

		/*-------+
		| slices |
		+-------*/

		/**
		 * @return A slice representing the row at @a index.
		 */
		RowSlice Row(size_type index);

		/**
		 * @return A slice representing the row at @a index.
		 */
		ConstRowSlice Row(size_type index) const;

		/**
		 * @return A slice representing the column at @a index.
		 */
		ColumnSlice Column(size_type index);

		/**
		 * @return A slice representing the column at @a index.
		 */
		ConstColumnSlice Column(size_type index) const;

		/**
		 * @return A slice representing the matrix's diagonal vector.
		 */
		DiagonalSlice Diagonal();

		/**
		 * @return A slice representing the matrix's diagonal vector.
		 */
		ConstDiagonalSlice Diagonal() const;

		/*------------------------+
		| slice factory functions |
		+------------------------*/

		private:
		/**
		 * A factory function that produces a @a RowSlice from a @a
		 * ColumnComponentIterator.  The iterator should point to the first
		 * component in a row.
		 */
		struct MakeRowSlice
		{
			typedef RowSlice result_type;
			typedef iterator argument_type;
			RowSlice operator ()(const ColumnComponentIterator &) const;
		};

		/**
		 * A factory function that produces a @a ConstRowSlice from a @a
		 * ConstColumnComponentIterator.  The iterator should point to the first
		 * component in a row.
		 */
		struct MakeConstRowSlice
		{
			typedef ConstRowSlice result_type;
			typedef const_iterator argument_type;
			ConstRowSlice operator ()(const ConstColumnComponentIterator &) const;
		};

		/**
		 * A factory function that produces a @a ColumnSlice from a @a
		 * RowComponentIterator. The iterator should point to the first
		 * component in a column.
		 */
		struct MakeColumnSlice
		{
			typedef ColumnSlice result_type;
			typedef iterator argument_type;
			ColumnSlice operator ()(const RowComponentIterator &) const;
		};

		/**
		 * A factory function that produces a @a ConstColumnSlice from a @a
		 * ConstRowComponentIterator.  The iterator should point to the first
		 * component in a column.
		 */
		struct MakeConstColumnSlice
		{
			typedef ConstColumnSlice result_type;
			typedef const_iterator argument_type;
			ConstColumnSlice operator ()(const ConstRowComponentIterator &) const;
		};

		/*---------------------+
		| slice-iterator types |
		+---------------------*/

		public:
		/**
		 * An iterator over slices representing the matrix's rows.
		 */
		typedef
			util::proxy_iterator<
				util::deferred_iterator<ColumnComponentIterator>,
				MakeRowSlice>
			RowIterator;

		/**
		 * A @c const iterator over slices representing the matrix's rows.
		 */
		typedef
			util::proxy_iterator<
				util::deferred_iterator<ConstColumnComponentIterator>,
				MakeConstRowSlice>
			ConstRowIterator;

		/**
		 * An iterator over slices representing the matrix's columns.
		 */
		typedef
			util::proxy_iterator<
				util::deferred_iterator<RowComponentIterator>,
				MakeColumnSlice>
			ColumnIterator;

		/**
		 * A @c const iterator over slices representing the matrix's columns.
		 */
		typedef
			util::proxy_iterator<
				util::deferred_iterator<ConstRowComponentIterator>,
				MakeConstColumnSlice>
			ConstColumnIterator;

		/*----------------+
		| slice iterators |
		+----------------*/

		/**
		 * @return An iterator over slices representing the matrix's rows,
		 * starting at the first row.
		 */
		RowIterator BeginRows();

		/**
		 * @return An iterator over slices representing the matrix's rows,
		 * starting at the first row.
		 */
		ConstRowIterator BeginRows() const;

		/**
		 * @return An iterator over slices representing the matrix's rows,
		 * starting at one past the last row.
		 */
		RowIterator EndRows();

		/**
		 * @return An iterator over slices representing the matrix's rows,
		 * starting at one past the last row.
		 */
		ConstRowIterator EndRows() const;

		/**
		 * @return An iterator over slices representing the matrix's columns,
		 * starting at to the first column.
		 */
		ColumnIterator BeginColumns();

		/**
		 * @return An iterator over slices representing the matrix's columns,
		 * starting at to the first column.
		 */
		ConstColumnIterator BeginColumns() const;

		/**
		 * @return An iterator over slices representing the matrix's columns,
		 * starting at to one past the last column.
		 */
		ColumnIterator EndColumns();

		/**
		 * @return An iterator over slices representing the matrix's columns,
		 * starting at to one past the last column.
		 */
		ConstColumnIterator EndColumns() const;

		/*-------------+
		| slice ranges |
		+-------------*/

		/**
		 * @return A range providing iteration over slices of the matrix's rows.
		 */
		boost::iterator_range<RowIterator> Rows();

		/**
		 * @return A range providing iteration over slices of the matrix's rows.
		 */
		boost::iterator_range<ConstRowIterator> Rows() const;

		/**
		 * @return A range providing iteration over slices of the matrix's
		 * columns.
		 */
		boost::iterator_range<ColumnIterator> Columns();

		/**
		 * @return A range providing iteration over slices of the matrix's
		 * columns.
		 */
		boost::iterator_range<ConstColumnIterator> Columns() const;
	};

////////// Matrix //////////////////////////////////////////////////////////////

	/**
	 * A matrix class.
	 */
	template <unsigned nr, unsigned nc, typename T>
		class Matrix : public BasicMatrix<nr, nc, T>
	{
		typedef BasicMatrix<nr, nc, T> Base;

		/*-------------+
		| constructors |
		+-------------*/

		public:
		/**
		 * @defgroup
		 * Diagonal matrix constructors.
		 * @{
		 */
		/**
		 * Constructs a scalar matrix.
		 */
		Matrix(T diagonal = 1);

		/**
		 * Constructs a diagonal matrix.
		 */
		explicit Matrix(const Vector<nr < nc ? nr : nc, T> &diagonal);
		///@}

		/*-------------+
		| data members |
		+-------------*/

		private:
		/**
		 * The matrix components, represented by an array.  The array is
		 * padded to make room for the end pointer in the major axis.
		 */
		T _data[nr * (nc + 1) - 1];
	};

////////// Matrix<1,n> /////////////////////////////////////////////////////////

	/**
	 * A single-row matrix class.
	 */
	template <unsigned n, typename T>
		class Matrix<1, n, T> : public BasicMatrix<1, n, T>
	{
		typedef BasicMatrix<1, n, T> Base;

		public:
		// FIXME: implement
	};

////////// Matrix<n,1> /////////////////////////////////////////////////////////

	/**
	 * A single-column matrix class.
	 */
	template <unsigned n, typename T>
		class Matrix<n, 1, T> : public BasicMatrix<n, 1, T>
	{
		typedef BasicMatrix<n, 1, T> Base;

		public:
		// FIXME: implement
	};

////////// Matrix<2,2> /////////////////////////////////////////////////////////

	/**
	 * A 2x2 matrix class.
	 */
	template <typename T>
		class Matrix<2, 2, T> : public BasicMatrix<2, 2, T>
	{
		typedef BasicMatrix<2, 2, T> Base;

		/*-------------+
		| constructors |
		+-------------*/

		public:
		/**
		 * @defgroup
		 * Diagonal matrix constructors.
		 * @{
		 */
		/**
		 * Constructs a scalar matrix.
		 */
		Matrix(T diagonal = 1);

		/**
		 * Constructs a diagonal matrix.
		 */
		explicit Matrix(const Vector<2, T> &diagonal);
		///@}

		/**
		 * @defgroup
		 * General matrix constructors.
		 * @{
		 */
		/**
		 * Constructs a matrix from a complete set of individual components.
		 */
		Matrix(
			T, T,
			T, T);

		/**
		 * Constructs a matrix from a complete set of row vectors.
		 */
		Matrix(
			const Vector<2, T> &,
			const Vector<2, T> &);
		///@}

		/*-------------+
		| data members |
		+-------------*/

		union
		{
			/**
			 * The matrix components, represented by variables.  The variables
			 * are named according to their coordinates, where the first
			 * character refers to the row and the second character refers to
			 * the column.
			 */
			__extension__ struct
			{
				T
					xx, yx,
					xy, yy;
			};

			/**
			 * The matrix components, represented by an array.  The array is
			 * padded to make room for the end pointer in the major axis.  This
			 * member is conceptually private.
			 */
			T _data[2 * (2 + 1) - 1];
		};
	};

////////// Matrix<3,3> /////////////////////////////////////////////////////////

	/**
	 * A 3x3 matrix class.
	 */
	template <typename T>
		class Matrix<3, 3, T> : public BasicMatrix<3, 3, T>
	{
		typedef BasicMatrix<3, 3, T> Base;

		/*-------------+
		| constructors |
		+-------------*/

		public:
		/**
		 * @defgroup
		 * Diagonal matrix constructors.
		 * @{
		 */
		/**
		 * Constructs a scalar matrix.
		 */
		Matrix(T diagonal = 1);

		/**
		 * Constructs a diagonal matrix.
		 */
		explicit Matrix(const Vector<3, T> &diagonal);
		///@}

		/**
		 * @defgroup
		 * General matrix constructors.
		 * @{
		 */
		/**
		 * Constructs a matrix from a complete set of individual components.
		 */
		Matrix(
			T, T, T,
			T, T, T,
			T, T, T);

		/**
		 * Constructs a matrix from a complete set of row vectors.
		 */
		Matrix(
			const Vector<3, T> &,
			const Vector<3, T> &,
			const Vector<3, T> &);
		///@}

		/**
		 * @defgroup
		 * Rotation matrix constructors.
		 * @{
		 */
		/**
		 * Constructs a rotation matrix from an axis/angle rotation.
		 */
		explicit Matrix(const Axan<T> &);

		/**
		 * Constructs a rotation matrix from a euler rotation.
		 */
		explicit Matrix(const Euler<T> &);

		/**
		 * Constructs a rotation matrix from a quaternion orientation.
		 */
		explicit Matrix(const Quat<T> &);
		///@}


		/*-------------+
		| data members |
		+-------------*/

		union
		{
			/**
			 * The matrix components, represented by variables.  The variables
			 * are named according to their coordinates, where the first
			 * character refers to the row and the second character refers to
			 * the column.
			 */
			__extension__ struct
			{
				T
					xx, yx, zx,
					xy, yy, zy,
					xz, yz, zz;
			};

			/**
			 * The matrix components, represented by an array.  The array is
			 * padded to make room for the end pointer in the major axis.  This
			 * member is conceptually private.
			 */
			T _data[3 * (3 + 1) - 1];
		};
	};

////////// Matrix<3,4> /////////////////////////////////////////////////////////

	/**
	 * A 3x4 matrix class.
	 */
	template <typename T>
		class Matrix<3, 4, T> : public BasicMatrix<3, 4, T>
	{
		typedef BasicMatrix<3, 4, T> Base;

		/*-------------+
		| constructors |
		+-------------*/

		public:
		/**
		 * @defgroup
		 * Diagonal matrix constructors.
		 * @{
		 */
		/**
		 * Constructs a scalar matrix.
		 */
		Matrix(T diagonal = 1);

		/**
		 * Constructs a diagonal matrix.
		 */
		explicit Matrix(const Vector<3, T> &);
		///@}

		/**
		 * @defgroup
		 * General matrix constructors.
		 * @{
		 */
		/**
		 * Constructs a matrix from a complete set of individual components.
		 */
		Matrix(
			T, T, T, T,
			T, T, T, T,
			T, T, T, T);

		/**
		 * Constructs a matrix from a complete set of row vectors.
		 */
		Matrix(
			const Vector<4, T> &,
			const Vector<4, T> &,
			const Vector<4, T> &);
		///@}

		/**
		 * @defgroup
		 * Rotation matrix constructors.
		 * @{
		 */
		/**
		 * Constructs a rotation matrix from an axis/angle rotation.
		 */
		explicit Matrix(const Axan<T> &);

		/**
		 * Constructs a rotation matrix from a euler rotation.
		 */
		explicit Matrix(const Euler<T> &);

		/**
		 * Constructs a rotation matrix from a quaternion orientation.
		 */
		explicit Matrix(const Quat<T> &);
		///@}

		/*-------------+
		| data members |
		+-------------*/

		union
		{
			/**
			 * The matrix components, represented by variables.  The variables
			 * are named according to their coordinates, where the first
			 * character refers to the row and the second character refers to
			 * the column.
			 */
			__extension__ struct
			{
				T
					xx, yx, zx,
					xy, yy, zy,
					xz, yz, zz,
					xw, yw, zw;
			};

			/**
			 * The matrix components, represented by an array.  The array is
			 * padded to make room for the end pointer in the major axis.  This
			 * member is conceptually private.
			 */
			T _data[3 * (4 + 1) - 1];
		};
	};

////////// Matrix<4,4> /////////////////////////////////////////////////////////

	/**
	 * A 4x4 matrix class.
	 */
	template <typename T>
		class Matrix<4, 4, T> : public BasicMatrix<4, 4, T>
	{
		typedef BasicMatrix<4, 4, T> Base;

		/*-------------+
		| constructors |
		+-------------*/

		public:
		/**
		 * @defgroup
		 * Diagonal matrix constructors.
		 * @{
		 */
		/**
		 * Constructs a scalar matrix.
		 */
		Matrix(T diagonal = 1);

		/**
		 * Constructs a diagonal matrix.
		 */
		explicit Matrix(const Vector<3, T> &diagonal);

		/**
		 * Constructs a diagonal matrix.
		 */
		explicit Matrix(const Vector<4, T> &diagonal);
		///@}

		/**
		 * @defgroup
		 * General matrix constructors.
		 * @{
		 */
		/**
		 * Constructs a matrix from a complete set of individual components.
		 */
		Matrix(
			T, T, T, T,
			T, T, T, T,
			T, T, T, T,
			T, T, T, T);

		/**
		 * Constructs a matrix from a complete set of row vectors.
		 */
		Matrix(
			const Vector<4, T> &,
			const Vector<4, T> &,
			const Vector<4, T> &,
			const Vector<4, T> &);
		///@}

		/**
		 * @defgroup
		 * Rotation matrix constructors.
		 * @{
		 */
		/**
		 * Constructs a rotation matrix from an axis/angle rotation.
		 */
		explicit Matrix(const Axan<T> &);

		/**
		 * Constructs a rotation matrix from a euler rotation.
		 */
		explicit Matrix(const Euler<T> &);

		/**
		 * Constructs a rotation matrix from a quaternion orientation.
		 */
		explicit Matrix(const Quat<T> &);
		///@}

		/*-------------+
		| data members |
		+-------------*/

		union
		{
			/**
			 * The matrix components, represented by variables.  The variables
			 * are named according to their coordinates, where the first
			 * character refers to the row and the second character refers to
			 * the column.
			 */
			__extension__ struct
			{
				T
					xx, yx, zx, wx,
					xy, yy, zy, wy,
					xz, yz, zz, wz,
					xw, yw, zw, ww;
			};

			/**
			 * The matrix components, represented by an array.  The array is
			 * padded to make room for the end pointer in the major axis.  This
			 * member is conceptually private.
			 */
			T _data[4 * (4 + 1) - 1];
		};
	};

////////// free functions //////////////////////////////////////////////////////

	/*---------------+
	| initialization |
	+---------------*/

	template <unsigned nr, unsigned nc, typename T = DefaultType> Matrix<nr, nc, T> ZeroMatrix();
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

	/*----------------+
	| unary operators |
	+----------------*/

	template <unsigned nr, unsigned nc, typename T> Matrix<nr, nc, T> operator +(const Matrix<nr, nc, T> &);
	template <unsigned nr, unsigned nc, typename T> Matrix<nr, nc, T> operator -(const Matrix<nr, nc, T> &);

	/*---------------------+
	| arithmetic operators |
	+---------------------*/

	template <unsigned nr, unsigned nc, typename T, typename U, unsigned n> Matrix<nr, nc, typename ArithmeticConversion<T, U>::Result> operator *(const Matrix<nr, n, T> &, const Matrix<n, nc, U> &);
	template <typename T, typename U> Matrix<3, 4, typename ArithmeticConversion<T, U>::Result> operator *(const Matrix<3, 4, T> &, const Matrix<3, 4, U> &);
	template <unsigned nr, unsigned nc, typename T, typename U> Matrix<nr, nc, typename ArithmeticConversion<T, U>::Result> operator +(const Matrix<nr, nc, T> &, const Matrix<nr, nc, U> &);
	template <unsigned nr, unsigned nc, typename T, typename U> Matrix<nr, nc, typename ArithmeticConversion<T, U>::Result> operator -(const Matrix<nr, nc, T> &, const Matrix<nr, nc, U> &);

	template <unsigned nr, unsigned nc, typename T, typename U> Matrix<nr, nc, typename ArithmeticConversion<T, U>::Result> operator *(const Matrix<nr, nc, T> &, U);
	template <unsigned nr, unsigned nc, typename T, typename U> Matrix<nr, nc, typename ArithmeticConversion<T, U>::Result> operator /(const Matrix<nr, nc, T> &, U);
	template <unsigned nr, unsigned nc, typename T, typename U> Matrix<nr, nc, typename ArithmeticConversion<T, U>::Result> operator +(const Matrix<nr, nc, T> &, U);
	template <unsigned nr, unsigned nc, typename T, typename U> Matrix<nr, nc, typename ArithmeticConversion<T, U>::Result> operator -(const Matrix<nr, nc, T> &, U);

	template <unsigned nr, unsigned nc, typename T, typename U> Matrix<nr, nc, typename ArithmeticConversion<T, U>::Result> operator *(T, const Matrix<nr, nc, U> &);
	template <unsigned nr, unsigned nc, typename T, typename U> Matrix<nr, nc, typename ArithmeticConversion<T, U>::Result> operator /(T, const Matrix<nr, nc, U> &);
	template <unsigned nr, unsigned nc, typename T, typename U> Matrix<nr, nc, typename ArithmeticConversion<T, U>::Result> operator +(T, const Matrix<nr, nc, U> &);
	template <unsigned nr, unsigned nc, typename T, typename U> Matrix<nr, nc, typename ArithmeticConversion<T, U>::Result> operator -(T, const Matrix<nr, nc, U> &);

	/*---------------------+
	| relational operators |
	+---------------------*/

	template <unsigned nr, unsigned nc, typename T, typename U> bool operator ==(const Matrix<nr, nc, T> &, const Matrix<nr, nc, U> &);
	template <unsigned nr, unsigned nc, typename T, typename U> bool operator !=(const Matrix<nr, nc, T> &, const Matrix<nr, nc, U> &);

	/*------------------+
	| logical operators |
	+------------------*/

	template <unsigned nr, unsigned nc> Matrix<nr, nc, bool> operator &&(const Matrix<nr, nc, bool> &, const Matrix<nr, nc, bool> &);
	template <unsigned nr, unsigned nc> Matrix<nr, nc, bool> operator ||(const Matrix<nr, nc, bool> &, const Matrix<nr, nc, bool> &);
	template <unsigned nr, unsigned nc> Matrix<nr, nc, bool> operator &&(const Matrix<nr, nc, bool> &, bool);
	template <unsigned nr, unsigned nc> Matrix<nr, nc, bool> operator ||(const Matrix<nr, nc, bool> &, bool);
	template <unsigned nr, unsigned nc> Matrix<nr, nc, bool> operator &&(bool, const Matrix<nr, nc, bool> &);
	template <unsigned nr, unsigned nc> Matrix<nr, nc, bool> operator ||(bool, const Matrix<nr, nc, bool> &);
	template <unsigned nr, unsigned nc> Matrix<nr, nc, bool> operator !(const Matrix<nr, nc, bool> &);

	/*------------------------------+
	| compound assignment operators |
	+------------------------------*/

	template <unsigned nr, unsigned nc, typename T, typename U> Matrix<nr, nc, T> &operator *=(Matrix<nr, nc, T> &, const Matrix<nc, nc, U> &);
	template <typename T, typename U> Matrix<3, 4, T> &operator *=(Matrix<3, 4, T> &, const Matrix<3, 4, U> &);
	template <unsigned nr, unsigned nc, typename T, typename U> Matrix<nr, nc, T> &operator +=(Matrix<nr, nc, T> &, const Matrix<nr, nc, U> &);
	template <unsigned nr, unsigned nc, typename T, typename U> Matrix<nr, nc, T> &operator -=(Matrix<nr, nc, T> &, const Matrix<nr, nc, U> &);

	template <unsigned nr, unsigned nc, typename T, typename U> Matrix<nr, nc, T> &operator *=(Matrix<nr, nc, T> &, U);
	template <unsigned nr, unsigned nc, typename T, typename U> Matrix<nr, nc, T> &operator /=(Matrix<nr, nc, T> &, U);
	template <unsigned nr, unsigned nc, typename T, typename U> Matrix<nr, nc, T> &operator +=(Matrix<nr, nc, T> &, U);
	template <unsigned nr, unsigned nc, typename T, typename U> Matrix<nr, nc, T> &operator -=(Matrix<nr, nc, T> &, U);

	/*--------------------------+
	| transformation extraction |
	+--------------------------*/

	/**
	 * Returns the translation component of an affine-transformation matrix.
	 */
	template <unsigned n, typename T>
		Vector<n, T> GetTranslation(const Matrix<n, n + 1, T> &);

	/**
	 * Returns the translation component of an affine-transformation matrix.
	 */
	template <unsigned n, typename T>
		Vector<n - 1, T> GetTranslation(const Matrix<n, n, T> &);

	/**
	 * Returns the rotation component of an affine-transformation matrix.
	 */
	template <typename T>
		T GetRotation(const Matrix<2, 2, T> &);

	/**
	 * Returns the rotation component of an affine-transformation matrix.
	 */
	template <unsigned nr, unsigned nc, typename T,
		ENABLE_IF(nr == nc && (nr == 3 || nr == 4) || nr == 3 && nc == 4)>
		Axan<T> GetRotation(const Matrix<nr, nc, T> &);

	/**
	 * Returns the rotation component of an affine-transformation matrix,
	 * in terms of an orientation.
	 */
	template <unsigned nr, unsigned nc, typename T,
		ENABLE_IF(nr == nc && (nr == 3 || nr == 4) || nr == 3 && nc == 4)>
		Quat<T> GetOrientation(const Matrix<nr, nc, T> &);

	/**
	 * Returns the scale component of an affine-transformation matrix.
	 */
	template <unsigned nr, unsigned nc, typename T>
		Vector<nr < nc ? nr : nc, T> GetScale(const Matrix<nr, nc, T> &);

	/**
	 * Returns the scale component of an affine-transformation matrix.
	 */
	template <typename T>
		Vector<3, T> GetScale(const Matrix<4, 4, T> &);

	/*-----------------+
	| other operations |
	+-----------------*/

	/**
	 * Returns a boolean matrix containing the results of calling Near()
	 * with each pair of elements from the two input matrices.
	 */
	template <unsigned nr, unsigned nc, typename T, typename U>
		Matrix<nr, nc, bool> Near(
			const Matrix<nr, nc, T> &,
			const Matrix<nr, nc, U> &);

	/**
	 * Transposes the matrix.
	 */
	template <unsigned nr, unsigned nc, typename T>
		Matrix<nc, nr, T> Tpos(const Matrix<nr, nc, T> &);

	/**
	 * Normalizes an affine-transformation matrix to produce a euclidean-
	 * transformation matrix.
	 */
	template <unsigned nr, unsigned nc, typename T>
		Matrix<nr, nc, T> Norm(Matrix<nr, nc, T>);

	/*--------------------+
	| AABB transformation |
	+--------------------*/

	/**
	 * Transforms a 3-dimensional AABB by a 3x3 affine-transformation
	 * matrix.
	 */
	template <typename T, typename U>
		Aabb<3, typename ArithmeticConversion<T, U>::Result>
		operator *(const Matrix<3, 3, T> &, const Aabb<3, U> &);

	/**
	 * Transforms a 3-dimensional AABB by a 3x4 affine-transformation
	 * matrix.
	 */
	template <typename T, typename U>
		Aabb<3, typename ArithmeticConversion<T, U>::Result>
		operator *(const Matrix<3, 4, T> &, const Aabb<3, U> &);

	/*----------------------+
	| vector transformation |
	+----------------------*/

	/**
	 * Transforms an n-dimensional vector by an mxn affine-transformation
	 * matrix.
	 */
	template <unsigned nr, unsigned nc, typename T, typename U>
		Vector<nr, typename ArithmeticConversion<T, U>::Result>
		operator *(const Matrix<nr, nc, T> &, const Vector<nc, U> &);

	/**
	 * Transforms a 3-dimensional vector by an 3x4 affine-transformation
	 * matrix.
	 */
	template <typename T, typename U>
		Vector<3, typename ArithmeticConversion<T, U>::Result>
		operator *(const Matrix<3, 4, T> &, const Vector<3, U> &);

	/**
	 * Transforms an n-dimensional vector by an nxm affine-transformation
	 * matrix.
	 */
	template <unsigned nr, unsigned nc, typename T, typename U>
		Vector<nc, typename ArithmeticConversion<T, U>::Result>
		operator *(const Vector<nr, T> &, const Matrix<nr, nc, U> &);

	/**
	 * Transforms an n-dimensional vector by an nxn affine-transformation
	 * matrix.
	 */
	template <unsigned n, typename T, typename U>
		Vector<n, T> &operator *=(Vector<n, T> &, const Matrix<n, n, U> &);

	/*----------------------------+
	| stream insertion/extraction |
	+----------------------------*/

	/**
	 * Serializes a matrix to a stream.
	 */
	template <typename Char, typename CharTraits, unsigned nr, unsigned nc, typename T>
		std::basic_ostream<Char, CharTraits> &operator <<(std::basic_ostream<Char, CharTraits> &, const Matrix<nr, nc, T> &);

	/**
	 * Deserializes a matrix from a stream.
	 */
	template <typename Char, typename CharTraits, unsigned nr, unsigned nc, typename T>
		std::basic_istream<Char, CharTraits> &operator >>(std::basic_istream<Char, CharTraits> &, Matrix<nr, nc, T> &);

	/*-------------------------------+
	| standard-library compatibility |
	+-------------------------------*/

	/**
	 * Swaps the contents of two matrices.
	 */
	template <unsigned nr, unsigned nc, typename T>
		void swap(Matrix<nr, nc, T> &, Matrix<nr, nc, T> &);
}}

#	include "Matrix.tpp"
#endif
