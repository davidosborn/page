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

#ifndef    page_math_Vector_hpp
#   define page_math_Vector_hpp

#	include <cstddef> // ptrdiff_t, size_t
#	include <iosfwd> // basic_[io]stream
#	include <iterator> // reverse_iterator
#	include <type_traits> // is_integral

#	include "../util/type_traits/sfinae.hpp" // ENABLE_IF
#	include "ArithmeticConversion.hpp"
#	include "float.hpp" // Real
#	include "fwd.hpp" // DefaultType, Vector

namespace page { namespace math
{
////////// BasicVector /////////////////////////////////////////////////////////

	/**
	 * A base class for @c Vector, providing a common implementation which is
	 * shared by all specializations.
	 */
	template <unsigned n, typename T>
		class BasicVector
	{
		/*----------------------+
		| container type-traits |
		+----------------------*/

		public:
		typedef T                                     value_type;
		typedef value_type &                          reference;
		typedef const value_type &                    const_reference;
		typedef value_type *                          iterator;
		typedef const value_type *                    const_iterator;
		typedef std::reverse_iterator<iterator>       reverse_iterator;
		typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
		typedef std::ptrdiff_t                        difference_type;
		typedef std::size_t                           size_type;

		/*-------------+
		| constructors |
		+-------------*/

		protected:
		BasicVector(T = 0);
		BasicVector(const BasicVector &);
		template <unsigned m, typename U>
			explicit BasicVector(const BasicVector<m, U> &);

		/*-----------+
		| assignment |
		+-----------*/

		BasicVector &operator =(T);
		BasicVector &operator =(const BasicVector &);

		/*----------+
		| iteration |
		+----------*/

		public:
		iterator       begin();
		const_iterator begin() const;
		iterator       end();
		const_iterator end() const;

		reverse_iterator       rbegin();
		const_reverse_iterator rbegin() const;
		reverse_iterator       rend();
		const_reverse_iterator rend() const;

		/*---------------+
		| element access |
		+---------------*/

		reference       operator [](size_type);
		const_reference operator [](size_type) const;
	};

////////// Vector //////////////////////////////////////////////////////////////

	/**
	 * A vector class template, parameterized by dimension and type.
	 */
	template <unsigned n, typename T>
		class Vector : public BasicVector<n, T>
	{
		typedef BasicVector<n, T> Base;

		/*-------------+
		| constructors |
		+-------------*/

		public:
		Vector(T = 0);
		template <unsigned m, typename U>
			explicit Vector(const Vector<m, U> &);

		/*-----------+
		| assignment |
		+-----------*/

		Vector &operator =(T);

		/*-----------+
		| conversion |
		+-----------*/

		template <typename U>
			operator Vector<n, U>() const;

		/*-------------+
		| data members |
		+-------------*/

		private:
		T _data[n];
	};

////////// Vector<2> ///////////////////////////////////////////////////////////

	/**
	 * A specialization of @c Vector with two dimensions.
	 */
	template <typename T>
		class Vector<2, T> : public BasicVector<2, T>
	{
		typedef BasicVector<2, T> Base;

		/*-------------+
		| constructors |
		+-------------*/

		public:
		Vector(T = 0);
		Vector(T, T);
		template <unsigned m, typename U>
			explicit Vector(const Vector<m, U> &);

		/*-----------+
		| assignment |
		+-----------*/

		Vector &operator =(T);

		/*-----------+
		| conversion |
		+-----------*/

		template <typename U>
			operator Vector<2, U>() const;

		/*-------------+
		| data members |
		+-------------*/

		union
		{
			__extension__ struct { T x, y; };
			T _data[2]; // HACK: conceptually private
		};
	};

////////// Vector<3> ///////////////////////////////////////////////////////////

	/**
	 * A specialization of @c Vector with three dimensions.
	 */
	template <typename T>
		struct Vector<3, T> : public BasicVector<3, T>
	{
		typedef BasicVector<3, T> Base;

		/*-------------+
		| constructors |
		+-------------*/

		public:
		Vector(T = 0);
		Vector(T, T, T);
		Vector(const Vector<2, T> &, T);
		template <unsigned m, typename U>
			explicit Vector(const Vector<m, U> &);

		/*-----------+
		| assignment |
		+-----------*/

		Vector &operator =(T);

		/*-----------+
		| conversion |
		+-----------*/

		template <typename U>
			operator Vector<3, U>() const;

		/*-------------+
		| data members |
		+-------------*/

		union
		{
			__extension__ struct { T x, y, z; };
			T _data[3]; // HACK: conceptually private
		};
	};

////////// Vector<4> ///////////////////////////////////////////////////////////

	/**
	 * A specialization of @c Vector with four dimensions.
	 */
	template <typename T>
		struct Vector<4, T> : public BasicVector<4, T>
	{
		typedef BasicVector<4, T> Base;

		/*-------------+
		| constructors |
		+-------------*/

		public:
		Vector(T = 0, T = 1);
		Vector(T, T, T, T = 1);
		Vector(const Vector<2, T> &, T, T = 1);
		Vector(const Vector<3, T> &, T);
		template <unsigned m, typename U>
			explicit Vector(const Vector<m, U> &);

		/*-----------+
		| assignment |
		+-----------*/

		Vector &operator =(T);

		/*-----------+
		| conversion |
		+-----------*/

		template <typename U>
			operator Vector<4, U>() const;

		/*-------------+
		| data members |
		+-------------*/

		union
		{
			__extension__ struct { T x, y, z, w; };
			T _data[4]; // HACK: conceptually private
		};
	};

////////// free functions //////////////////////////////////////////////////////

	/*---------------+
	| initialization |
	+---------------*/

	/**
	 * Returns a vector filled with zeros.
	 */
	template <unsigned n, typename T = DefaultType,
		ENABLE_IF(n != 4)>
		Vector<n, T> ZeroVector();

	/**
	 * Returns a 4-dimensional vector filled with zeros, with the last component
	 * defaulting to 1.
	 */
	template <unsigned n, typename T = DefaultType,
		ENABLE_IF(n == 4)>
		Vector<n, T> ZeroVector(T = 1);

	/**
	 * Returns a vector filled with ones.
	 */
	template <unsigned n, typename T = DefaultType,
		ENABLE_IF(n != 4)>
		Vector<n, T> ScaleVector();

	/**
	 * Returns a 4-dimensional vector filled with ones, with the last component
	 * defaulting to 1.
	 */
	template <unsigned n, typename T = DefaultType,
		ENABLE_IF(n == 4)>
		Vector<n, T> ScaleVector(T = 1);

	/**
	 * Returns a unit vector pointing along the Z axis.
	 */
	template <unsigned n, typename T = DefaultType,
		ENABLE_IF(n != 4)>
		Vector<n, T> NormVector();

	/**
	 * Returns a 4-dimensional unit vector pointing along the Z axis, with the
	 * last component defaulting to 0.
	 */
	template <unsigned n, typename T = DefaultType,
		ENABLE_IF(n == 4)>
		Vector<n, T> NormVector(T = 0);

	/**
	 * Returns a unit vector pointing along the Y axis.
	 */
	template <unsigned n, typename T = DefaultType,
		ENABLE_IF(n == 2 || n == 3)>
		Vector<n, T> UpVector();

	/**
	 * Returns a 4-dimensional unit vector pointing along the Y axis, with the
	 * last component defaulting to 0.
	 */
	template <unsigned n, typename T = DefaultType,
		ENABLE_IF(n == 4)>
		Vector<n, T> UpVector(T = 0);

	/*----------------+
	| unary operators |
	+----------------*/

	template <unsigned n, typename T> Vector<n, T> &operator ++(Vector<n, T> &);
	template <unsigned n, typename T> Vector<n, T> &operator --(Vector<n, T> &);

	template <unsigned n, typename T> Vector<n, T> operator ++(Vector<n, T> &, int);
	template <unsigned n, typename T> Vector<n, T> operator --(Vector<n, T> &, int);

	template <unsigned n, typename T> Vector<n, T> operator +(const Vector<n, T> &);
	template <unsigned n, typename T> Vector<n, T> operator -(const Vector<n, T> &);

	/*---------------------+
	| arithmetic operators |
	+---------------------*/

	template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator *(const Vector<n, T> &, const Vector<n, U> &);
	template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator /(const Vector<n, T> &, const Vector<n, U> &);
	template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator %(const Vector<n, T> &, const Vector<n, U> &);
	template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator +(const Vector<n, T> &, const Vector<n, U> &);
	template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator -(const Vector<n, T> &, const Vector<n, U> &);

	template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator *(const Vector<n, T> &, U);
	template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator /(const Vector<n, T> &, U);
	template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator %(const Vector<n, T> &, U);
	template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator +(const Vector<n, T> &, U);
	template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator -(const Vector<n, T> &, U);

	template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator *(T, const Vector<n, U> &);
	template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator /(T, const Vector<n, U> &);
	template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator %(T, const Vector<n, U> &);
	template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator +(T, const Vector<n, U> &);
	template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator -(T, const Vector<n, U> &);

	/*----------------+
	| shift operators |
	+----------------*/

	template <unsigned n, typename T> Vector<n, T> operator <<(const Vector<n, T> &, const Vector<n, unsigned> &);
	template <unsigned n, typename T> Vector<n, T> operator >>(const Vector<n, T> &, const Vector<n, unsigned> &);

	template <unsigned n, typename T> Vector<n, T> operator <<(const Vector<n, T> &, unsigned);
	template <unsigned n, typename T> Vector<n, T> operator >>(const Vector<n, T> &, unsigned);

	template <unsigned n, typename T> typename std::enable_if<std::is_integral<T>::value, Vector<n, T>>::type operator <<(T, const Vector<n, unsigned> &);
	template <unsigned n, typename T> typename std::enable_if<std::is_integral<T>::value, Vector<n, T>>::type operator >>(T, const Vector<n, unsigned> &);

	/*---------------------+
	| relational operators |
	+---------------------*/

	template <unsigned n, typename T, typename U> Vector<n, bool> operator < (const Vector<n, T> &, const Vector<n, U> &);
	template <unsigned n, typename T, typename U> Vector<n, bool> operator > (const Vector<n, T> &, const Vector<n, U> &);
	template <unsigned n, typename T, typename U> Vector<n, bool> operator <=(const Vector<n, T> &, const Vector<n, U> &);
	template <unsigned n, typename T, typename U> Vector<n, bool> operator >=(const Vector<n, T> &, const Vector<n, U> &);
	template <unsigned n, typename T, typename U> Vector<n, bool> operator ==(const Vector<n, T> &, const Vector<n, U> &);
	template <unsigned n, typename T, typename U> Vector<n, bool> operator !=(const Vector<n, T> &, const Vector<n, U> &);

	template <unsigned n, typename T, typename U> Vector<n, bool> operator < (const Vector<n, T> &, U);
	template <unsigned n, typename T, typename U> Vector<n, bool> operator > (const Vector<n, T> &, U);
	template <unsigned n, typename T, typename U> Vector<n, bool> operator <=(const Vector<n, T> &, U);
	template <unsigned n, typename T, typename U> Vector<n, bool> operator >=(const Vector<n, T> &, U);
	template <unsigned n, typename T, typename U> Vector<n, bool> operator ==(const Vector<n, T> &, U);
	template <unsigned n, typename T, typename U> Vector<n, bool> operator !=(const Vector<n, T> &, U);

	template <unsigned n, typename T, typename U> Vector<n, bool> operator < (T, const Vector<n, U> &);
	template <unsigned n, typename T, typename U> Vector<n, bool> operator > (T, const Vector<n, U> &);
	template <unsigned n, typename T, typename U> Vector<n, bool> operator <=(T, const Vector<n, U> &);
	template <unsigned n, typename T, typename U> Vector<n, bool> operator >=(T, const Vector<n, U> &);
	template <unsigned n, typename T, typename U> Vector<n, bool> operator ==(T, const Vector<n, U> &);
	template <unsigned n, typename T, typename U> Vector<n, bool> operator !=(T, const Vector<n, U> &);

	/*------------------+
	| bitwise operators |
	+------------------*/

	template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator &(const Vector<n, T> &, const Vector<n, U> &);
	template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator ^(const Vector<n, T> &, const Vector<n, U> &);
	template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator |(const Vector<n, T> &, const Vector<n, U> &);

	template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator &(const Vector<n, T> &, U);
	template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator ^(const Vector<n, T> &, U);
	template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator |(const Vector<n, T> &, U);

	template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator &(T, const Vector<n, U> &);
	template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator ^(T, const Vector<n, U> &);
	template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator |(T, const Vector<n, U> &);

	/*------------------+
	| logical operators |
	+------------------*/

	template <unsigned n, typename T, typename U> Vector<n, bool> operator &&(const Vector<n, T> &, const Vector<n, U> &);
	template <unsigned n, typename T, typename U> Vector<n, bool> operator ||(const Vector<n, T> &, const Vector<n, U> &);

	template <unsigned n, typename T, typename U> Vector<n, bool> operator &&(const Vector<n, T> &, U);
	template <unsigned n, typename T, typename U> Vector<n, bool> operator ||(const Vector<n, T> &, U);

	template <unsigned n, typename T, typename U> Vector<n, bool> operator &&(T, const Vector<n, U> &);
	template <unsigned n, typename T, typename U> Vector<n, bool> operator ||(T, const Vector<n, U> &);

	template <unsigned n, typename T> Vector<n, bool> operator !(const Vector<n, T> &);

	/*------------------------------+
	| compound assignment operators |
	+------------------------------*/

	template <unsigned n, typename T, typename U> Vector<n, T> &operator *= (Vector<n, T> &, const Vector<n, U> &);
	template <unsigned n, typename T, typename U> Vector<n, T> &operator /= (Vector<n, T> &, const Vector<n, U> &);
	template <unsigned n, typename T, typename U> Vector<n, T> &operator %= (Vector<n, T> &, const Vector<n, U> &);
	template <unsigned n, typename T, typename U> Vector<n, T> &operator += (Vector<n, T> &, const Vector<n, U> &);
	template <unsigned n, typename T, typename U> Vector<n, T> &operator -= (Vector<n, T> &, const Vector<n, U> &);
	template <unsigned n, typename T>             Vector<n, T> &operator <<=(Vector<n, T> &, const Vector<n, unsigned> &);
	template <unsigned n, typename T>             Vector<n, T> &operator >>=(Vector<n, T> &, const Vector<n, unsigned> &);
	template <unsigned n, typename T, typename U> Vector<n, T> &operator &= (Vector<n, T> &, const Vector<n, U> &);
	template <unsigned n, typename T, typename U> Vector<n, T> &operator ^= (Vector<n, T> &, const Vector<n, U> &);
	template <unsigned n, typename T, typename U> Vector<n, T> &operator |= (Vector<n, T> &, const Vector<n, U> &);

	template <unsigned n, typename T, typename U> Vector<n, T> &operator *= (Vector<n, T> &, U);
	template <unsigned n, typename T, typename U> Vector<n, T> &operator /= (Vector<n, T> &, U);
	template <unsigned n, typename T, typename U> Vector<n, T> &operator %= (Vector<n, T> &, U);
	template <unsigned n, typename T, typename U> Vector<n, T> &operator += (Vector<n, T> &, U);
	template <unsigned n, typename T, typename U> Vector<n, T> &operator -= (Vector<n, T> &, U);
	template <unsigned n, typename T>             Vector<n, T> &operator <<=(Vector<n, T> &, unsigned);
	template <unsigned n, typename T>             Vector<n, T> &operator >>=(Vector<n, T> &, unsigned);
	template <unsigned n, typename T, typename U> Vector<n, T> &operator &= (Vector<n, T> &, U);
	template <unsigned n, typename T, typename U> Vector<n, T> &operator ^= (Vector<n, T> &, U);
	template <unsigned n, typename T, typename U> Vector<n, T> &operator |= (Vector<n, T> &, U);

	/*------------------+
	| boolean combiners |
	+------------------*/

	/**
	 * Returns @c true if all elements in the vector evaluate to @c true.
	 */
	template <unsigned n, typename T> bool All(const Vector<n, T> &);

	/**
	 * Returns @c true if any elements in the vector evaluate to @c true.
	 */
	template <unsigned n, typename T> bool Any(const Vector<n, T> &);

	/*----------------+
	| minimum/maximum |
	+----------------*/

	template <unsigned n, typename T> T Min(const Vector<n, T> &);
	template <unsigned n, typename T> T Max(const Vector<n, T> &);

	template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> Min(const Vector<n, T> &, const Vector<n, U> &);
	template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> Max(const Vector<n, T> &, const Vector<n, U> &);

	template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> Min(const Vector<n, T> &, U);
	template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> Max(const Vector<n, T> &, U);

	template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> Min(T, const Vector<n, U> &);
	template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> Max(T, const Vector<n, U> &);

	/*--------+
	| swizzle |
	+--------*/

	/**
	 * Builds a vector, taking an element from the @a source vector at each
	 * index in @a indices.
	 *
	 * @note The types in @a Indices are expected to be integral.
	 */
	template <unsigned n, typename T, typename... Indices>
		Vector<sizeof...(Indices), T>
		Swizzle(
			const Vector<n, T> &source,
			Indices... indices);

	/*---------------------+
	| tolerant equivalence |
	+---------------------*/

	template <unsigned n, typename T, typename U> Vector<n, bool> Near(const Vector<n, T> &, const Vector<n, U> &);
	template <unsigned n, typename T, typename U> Vector<n, bool> Near(const Vector<n, T> &, U);
	template <unsigned n, typename T, typename U> Vector<n, bool> Near(T, const Vector<n, U> &);

	/*-------------+
	| quantization |
	+-------------*/

	template <unsigned n, typename T> Vector<n, T> Round(const Vector<n, T> &);
	template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> Round(const Vector<n, T> &, U precision);
	template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> Round(const Vector<n, T> &, const Vector<n, U> &precision);

	/*-----------------+
	| other operations |
	+-----------------*/

	template <unsigned n, typename T> Vector<n, T> Abs(const Vector<n, T> &);
	template <typename T, typename U> typename ArithmeticConversion<T, U>::Result AngleBetween(const Vector<2, T> &, const Vector<2, U> &);
	template <typename T, typename U> typename ArithmeticConversion<T, U>::Result AngleBetween(const Vector<3, T> &, const Vector<3, U> &);
	template <typename T> typename Real<T>::type Aspect(const Vector<2, T> &);
	template <unsigned n, typename T> Vector<n, T> Ceil(const Vector<n, T> &);
	template <unsigned n, typename T> T Content(const Vector<n, T> &);
	template <typename T> Vector<2, T> Cross(const Vector<2, T> &);
	template <typename T, typename U> Vector<3, typename ArithmeticConversion<T, U>::Result> Cross(const Vector<3, T> &, const Vector<3, U> &);
	template <unsigned n, typename T, typename U> typename ArithmeticConversion<T, U>::Result Dot(const Vector<n, T> &, const Vector<n, U> &);
	template <unsigned n, typename T> Vector<n, T> Floor(const Vector<n, T> &);
	template <unsigned n, typename T> T Len(const Vector<n, T> &);
	template <unsigned n, typename T> Vector<n, T> Norm(const Vector<n, T> &);
	template <typename T, typename U> typename ArithmeticConversion<T, U>::Result PerpDot(const Vector<2, T> &, const Vector<2, U> &);
	template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> Proj(const Vector<n, T> &, const Vector<n, U> &);
	template <typename T, typename U> Vector<2, typename ArithmeticConversion<T, U>::Result> Rotate(const Vector<2, T> &, U);
	template <unsigned n, typename T> T Sum(const Vector<n, T> &);
	template <typename T> Vector<2, T> Transpose(const Vector<2, T> &);

	/*---------------------------------+
	| coordinate-system transformation |
	+---------------------------------*/

	/**
	 * Performs a barycentric coordinate transformation.
	 *
	 * @note The implementation of this function is based on math from the book,
	 *       "Real-Time Collision Detection", Christer Ericson, 2005.
	 */
	template <typename T> Vector<2, T> Barycentric(const Vector<2, T> &, const Vector<2, T> &, const Vector<2, T> &, const Vector<2, T> &point);

	/*----------------------------+
	| stream insertion/extraction |
	+----------------------------*/

	template <typename Char, typename CharTraits, unsigned n, typename T> std::basic_ostream<Char, CharTraits> &operator <<(std::basic_ostream<Char, CharTraits> &, const Vector<n, T> &);
	template <typename Char, typename CharTraits, unsigned n, typename T> std::basic_istream<Char, CharTraits> &operator >>(std::basic_istream<Char, CharTraits> &, Vector<n, T> &);

	/*-------------------------------+
	| standard-library compatibility |
	+-------------------------------*/

	template <unsigned n, typename T> void swap(Vector<n, T> &, Vector<n, T> &);
}}

#	include "Vector.tpp"
#endif
