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

#ifndef    page_math_Aabb_hpp
#   define page_math_Aabb_hpp

#	include <iosfwd> // basic_[io]stream

#	include "ArithmeticConversion.hpp"
#	include "float.hpp" // Real
#	include "fwd.hpp" // Aabb, DefaultType
#	include "Vector.hpp"

namespace page { namespace math
{
	/**
	 * An axis-aligned bounding-box class.
	 */
	template <unsigned n, typename T> struct Aabb
	{
		/*-------------+
		| constructors |
		+-------------*/

		Aabb(T = 0);
		explicit Aabb(const Vector<n, T> &);
		explicit Aabb(T, T);
		Aabb(const Vector<n, T> &, const Vector<n, T> &);
		template <unsigned m, typename U> explicit Aabb(const Aabb<m, U> &);

		/*-----------+
		| conversion |
		+-----------*/

		template <typename U> operator Aabb<n, U>() const;

		Vector<n, T> min, max;
	};

	/*---------------+
	| initialization |
	+---------------*/

	template <unsigned n, typename T, typename U> Aabb<n, typename ArithmeticConversion<T, U>::Result> AabbPositionSize(const Vector<n, T> &position, const Vector<n, U> &size);
	template <unsigned n, typename T = DefaultType> Aabb<n, T> InverseInfiniteAabb();
	template <unsigned n, typename T = DefaultType> Aabb<n, T> UnitAabb();

	/*----------------+
	| unary operators |
	+----------------*/

	template <unsigned n, typename T> Aabb<n, T> operator +(const Aabb<n, T> &);
	template <unsigned n, typename T> Aabb<n, T> operator -(const Aabb<n, T> &);

	/*----------------------------+
	| vector arithmetic operators |
	+----------------------------*/

	template <unsigned n, typename T, typename U> Aabb<n, typename ArithmeticConversion<T, U>::Result> operator *(const Aabb<n, T> &, const Vector<n, U> &);
	template <unsigned n, typename T, typename U> Aabb<n, typename ArithmeticConversion<T, U>::Result> operator /(const Aabb<n, T> &, const Vector<n, U> &);
	template <unsigned n, typename T, typename U> Aabb<n, typename ArithmeticConversion<T, U>::Result> operator +(const Aabb<n, T> &, const Vector<n, U> &);
	template <unsigned n, typename T, typename U> Aabb<n, typename ArithmeticConversion<T, U>::Result> operator -(const Aabb<n, T> &, const Vector<n, U> &);

	/*------------------------------------+
	| reverse vector arithmetic operators |
	+------------------------------------*/

	template <unsigned n, typename T, typename U> Aabb<n, typename ArithmeticConversion<T, U>::Result> operator *(const Vector<n, T> &, const Aabb<n, U> &);
	template <unsigned n, typename T, typename U> Aabb<n, typename ArithmeticConversion<T, U>::Result> operator /(const Vector<n, T> &, const Aabb<n, U> &);
	template <unsigned n, typename T, typename U> Aabb<n, typename ArithmeticConversion<T, U>::Result> operator +(const Vector<n, T> &, const Aabb<n, U> &);
	template <unsigned n, typename T, typename U> Aabb<n, typename ArithmeticConversion<T, U>::Result> operator -(const Vector<n, T> &, const Aabb<n, U> &);

	/*----------------------------+
	| scalar arithmetic operators |
	+----------------------------*/

	template <unsigned n, typename T, typename U> Aabb<n, typename ArithmeticConversion<T, U>::Result> operator *(const Aabb<n, T> &, U);
	template <unsigned n, typename T, typename U> Aabb<n, typename ArithmeticConversion<T, U>::Result> operator /(const Aabb<n, T> &, U);
	template <unsigned n, typename T, typename U> Aabb<n, typename ArithmeticConversion<T, U>::Result> operator +(const Aabb<n, T> &, U);
	template <unsigned n, typename T, typename U> Aabb<n, typename ArithmeticConversion<T, U>::Result> operator -(const Aabb<n, T> &, U);

	/*------------------------------------+
	| reverse scalar arithmetic operators |
	+------------------------------------*/

	template <unsigned n, typename T, typename U> Aabb<n, typename ArithmeticConversion<T, U>::Result> operator *(T, const Aabb<n, U> &);
	template <unsigned n, typename T, typename U> Aabb<n, typename ArithmeticConversion<T, U>::Result> operator /(T, const Aabb<n, U> &);
	template <unsigned n, typename T, typename U> Aabb<n, typename ArithmeticConversion<T, U>::Result> operator +(T, const Aabb<n, U> &);
	template <unsigned n, typename T, typename U> Aabb<n, typename ArithmeticConversion<T, U>::Result> operator -(T, const Aabb<n, U> &);

	/*---------------------+
	| relational operators |
	+---------------------*/

	template <unsigned n, typename T, typename U> bool operator ==(const Aabb<n, T> &, const Aabb<n, U> &);
	template <unsigned n, typename T, typename U> bool operator !=(const Aabb<n, T> &, const Aabb<n, U> &);

	/*----------------------------+
	| vector assignment operators |
	+----------------------------*/

	template <unsigned n, typename T, typename U> Aabb<n, T> &operator *=(Aabb<n, T> &, const Vector<n, U> &);
	template <unsigned n, typename T, typename U> Aabb<n, T> &operator /=(Aabb<n, T> &, const Vector<n, U> &);
	template <unsigned n, typename T, typename U> Aabb<n, T> &operator +=(Aabb<n, T> &, const Vector<n, U> &);
	template <unsigned n, typename T, typename U> Aabb<n, T> &operator -=(Aabb<n, T> &, const Vector<n, U> &);

	/*----------------------------+
	| scalar assignment operators |
	+----------------------------*/

	template <unsigned n, typename T, typename U> Aabb<n, T> &operator *=(Aabb<n, T> &, U);
	template <unsigned n, typename T, typename U> Aabb<n, T> &operator /=(Aabb<n, T> &, U);
	template <unsigned n, typename T, typename U> Aabb<n, T> &operator +=(Aabb<n, T> &, U);
	template <unsigned n, typename T, typename U> Aabb<n, T> &operator -=(Aabb<n, T> &, U);

	/*--------+
	| min/max |
	+--------*/

	template <unsigned n, typename T, typename U> Aabb<n, typename ArithmeticConversion<T, U>::Result> Min(const Aabb<n, T> &, const Aabb<n, U> &);
	template <unsigned n, typename T, typename U> Aabb<n, typename ArithmeticConversion<T, U>::Result> Max(const Aabb<n, T> &, const Aabb<n, U> &);
	template <unsigned n, typename T, typename U> Aabb<n, typename ArithmeticConversion<T, U>::Result> Max(const Aabb<n, T> &, const Vector<n, U> &);
	template <unsigned n, typename T, typename U> Aabb<n, typename ArithmeticConversion<T, U>::Result> Max(const Vector<n, T> &, const Aabb<n, U> &);

	/*------------+
	| grow/shrink |
	+------------*/

	template <unsigned n, typename T, typename U> Aabb<n, typename ArithmeticConversion<T, U>::Result> Grow(const Aabb<n, T> &, U);
	template <unsigned n, typename T, typename U> Aabb<n, typename ArithmeticConversion<T, U>::Result> Grow(const Aabb<n, T> &, const Vector<n, U> &);
	template <unsigned n, typename T, typename U> Aabb<n, typename ArithmeticConversion<T, U>::Result> Grow(const Aabb<n, T> &, U min, U max);
	template <unsigned n, typename T, typename U> Aabb<n, typename ArithmeticConversion<T, U>::Result> Grow(const Aabb<n, T> &, const Vector<n, U> &min, const Vector<n, U> &max);
	template <unsigned n, typename T, typename U> Aabb<n, typename ArithmeticConversion<T, U>::Result> Shrink(const Aabb<n, T> &, U);
	template <unsigned n, typename T, typename U> Aabb<n, typename ArithmeticConversion<T, U>::Result> Shrink(const Aabb<n, T> &, const Vector<n, U> &);
	template <unsigned n, typename T, typename U> Aabb<n, typename ArithmeticConversion<T, U>::Result> Shrink(const Aabb<n, T> &, U min, U max);
	template <unsigned n, typename T, typename U> Aabb<n, typename ArithmeticConversion<T, U>::Result> Shrink(const Aabb<n, T> &, const Vector<n, U> &min, const Vector<n, U> &max);

	/*------------+
	| containment |
	+------------*/

	template <unsigned n, typename T, typename U> bool Contains(const Aabb<n, T> &, const Aabb<n, U> &);
	template <unsigned n, typename T, typename U> bool Contains(const Aabb<n, T> &, const Vector<n, U> &);
	template <unsigned n, typename T, typename U> bool ContainsExclusive(const Aabb<n, T> &, const Aabb<n, U> &);
	template <unsigned n, typename T, typename U> bool ContainsExclusive(const Aabb<n, T> &, const Vector<n, U> &);
	template <unsigned n, typename T, typename U> bool ContainsMinExclusive(const Aabb<n, T> &, const Aabb<n, U> &);
	template <unsigned n, typename T, typename U> bool ContainsMinExclusive(const Aabb<n, T> &, const Vector<n, U> &);
	template <unsigned n, typename T, typename U> bool ContainsMaxExclusive(const Aabb<n, T> &, const Aabb<n, U> &);
	template <unsigned n, typename T, typename U> bool ContainsMaxExclusive(const Aabb<n, T> &, const Vector<n, U> &);

	/*---------------------------------+
	| spatial reference transformation |
	+---------------------------------*/

	template <unsigned n, typename T, typename U> Aabb<n, typename Real<typename ArithmeticConversion<T, U>::Result>::type> EnterSpace(const Aabb<n, T> &, const Aabb<n, U> &);
	template <unsigned n, typename T, typename U> Vector<n, typename Real<typename ArithmeticConversion<T, U>::Result>::type> EnterSpace(const Aabb<n, T> &, const Vector<n, U> &);
	template <unsigned n, typename T, typename U> Aabb<n, typename ArithmeticConversion<T, U>::Result> LeaveSpace(const Aabb<n, T> &, const Aabb<n, U> &);
	template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> LeaveSpace(const Aabb<n, T> &, const Vector<n, U> &);

	/*-----------------+
	| other operations |
	+-----------------*/

	template <typename T> typename Real<T>::type Aspect(const Aabb<2, T> &);
	template <unsigned n, typename T> Aabb<n, T> Ceil(const Aabb<n, T> &);
	template <unsigned n, typename T> Vector<n, T> Center(const Aabb<n, T> &);
	template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> Center(const Aabb<n, T> &, const Vector<n, U> &);
	template <unsigned n, typename T> Aabb<n, T> Floor(const Aabb<n, T> &);
	template <unsigned n, typename T> Vector<n, T> Size(const Aabb<n, T> &);

	/*----------------------------+
	| stream insertion/extraction |
	+----------------------------*/

	template <typename Char, typename CharTraits, unsigned n, typename T> std::basic_ostream<Char, CharTraits> &operator <<(std::basic_ostream<Char, CharTraits> &, const Aabb<n, T> &);
	template <typename Char, typename CharTraits, unsigned n, typename T> std::basic_istream<Char, CharTraits> &operator >>(std::basic_istream<Char, CharTraits> &, Aabb<n, T> &);

	/*-------------------------------+
	| standard-library compatibility |
	+-------------------------------*/

	template <unsigned n, typename T> void swap(Aabb<n, T> &, Aabb<n, T> &);
}}

#	include "Aabb.tpp"
#endif
