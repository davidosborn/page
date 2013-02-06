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

#ifndef    page_local_math_Quat_hpp
#   define page_local_math_Quat_hpp

#	include <cstddef> // ptrdiff_t, size_t
#	include <iosfwd> // [io]stream
#	include <iterator> // reverse_iterator
#	include "ArithmeticConversion.hpp"
#	include "fwd.hpp" // Axan, Euler, Matrix, Quat
#	include "Vector.hpp"

namespace page
{
	namespace math
	{
		/**
		 * A unit-quaternion class.
		 *
		 * @note Breaking from tradition, w is the last component.
		 */
		template <typename T> struct Quat
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

			// constructors
			Quat();
			Quat(T, T, T);
			Quat(T, T, T, T);
			template <typename U> explicit Quat(const Quat<U> &);
			explicit Quat(const Axan<T> &);
			explicit Quat(const Euler<T> &);
			explicit Quat(const Matrix<3, 3, T> &);
			explicit Quat(const Vector<3, T> &);
			explicit Quat(const Vector<4, T> &);
			Quat(const Vector<3, T> &, const Vector<3, T> &);

			// conversion
			template <typename U> operator Quat<U>() const;
			explicit operator Vector<3, T>() const;
			explicit operator Vector<4, T>() const;

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
			reference operator [](size_type);
			const_reference operator [](size_type) const;

			// zero equivalence
			operator void *() const;

			union
			{
				__extension__ struct { T x, y, z, w; };
				T _data[4]; // HACK: conceptually private
			};
		};

		// unary operators
		template <typename T> Quat<T> &operator ++(Quat<T> &);
		template <typename T> Quat<T> &operator --(Quat<T> &);
		template <typename T> Quat<T> operator ++(Quat<T> &, int);
		template <typename T> Quat<T> operator --(Quat<T> &, int);
		template <typename T> Quat<T> operator +(const Quat<T> &);
		template <typename T> Quat<T> operator -(const Quat<T> &);

		// arithmetic operators
		template <typename T, typename U> Quat<typename ArithmeticConversion<T, U>::Result> operator *(const Quat<T> &, const Quat<U> &);
		template <typename T, typename U> Quat<typename ArithmeticConversion<T, U>::Result> operator /(const Quat<T> &, const Quat<U> &);
		template <typename T, typename U> Quat<typename ArithmeticConversion<T, U>::Result> operator +(const Quat<T> &, const Quat<U> &);
		template <typename T, typename U> Quat<typename ArithmeticConversion<T, U>::Result> operator -(const Quat<T> &, const Quat<U> &);

		// scalar arithmetic operators
		template <typename T, typename U> Quat<typename ArithmeticConversion<T, U>::Result> operator *(const Quat<T> &, U);
		template <typename T, typename U> Quat<typename ArithmeticConversion<T, U>::Result> operator /(const Quat<T> &, U);
		template <typename T, typename U> Quat<typename ArithmeticConversion<T, U>::Result> operator +(const Quat<T> &, U);
		template <typename T, typename U> Quat<typename ArithmeticConversion<T, U>::Result> operator -(const Quat<T> &, U);

		// reverse scalar arithmetic operators
		template <typename T, typename U> Quat<typename ArithmeticConversion<T, U>::Result> operator *(T, const Quat<U> &);
		template <typename T, typename U> Quat<typename ArithmeticConversion<T, U>::Result> operator /(T, const Quat<U> &);
		template <typename T, typename U> Quat<typename ArithmeticConversion<T, U>::Result> operator +(T, const Quat<U> &);
		template <typename T, typename U> Quat<typename ArithmeticConversion<T, U>::Result> operator -(T, const Quat<U> &);

		// relational operators
		template <typename T, typename U> bool operator ==(const Quat<T> &, const Quat<U> &);
		template <typename T, typename U> bool operator !=(const Quat<T> &, const Quat<U> &);

		// assignment operators
		template <typename T, typename U> Quat<T> &operator *=(Quat<T> &, const Quat<U> &);
		template <typename T, typename U> Quat<T> &operator /=(Quat<T> &, const Quat<U> &);
		template <typename T, typename U> Quat<T> &operator +=(Quat<T> &, const Quat<U> &);
		template <typename T, typename U> Quat<T> &operator -=(Quat<T> &, const Quat<U> &);

		// scalar assignment operators
		template <typename T, typename U> Quat<T> &operator *=(Quat<T> &, U);
		template <typename T, typename U> Quat<T> &operator /=(Quat<T> &, U);
		template <typename T, typename U> Quat<T> &operator +=(Quat<T> &, U);
		template <typename T, typename U> Quat<T> &operator -=(Quat<T> &, U);

		// other operations
		template <typename T, typename U> Quat<typename ArithmeticConversion<T, U>::Result> Between(const Quat<T> &, const Quat<U> &);
		template <typename T, typename U> typename ArithmeticConversion<T, U>::Result Dot(const Quat<T> &, const Quat<U> &);
		template <typename T> Quat<T> Inv(const Quat<T> &);
		template <typename T> T Len(const Quat<T> &);
		template <typename T, typename U> bool Near(const Quat<T> &, const Quat<U> &);
		template <typename T> Quat<T> Norm(const Quat<T> &);

		// axis-angle transformation
		template <typename T, typename U> Quat<typename ArithmeticConversion<T, U>::Result> operator *(const Quat<T> &, const Axan<U> &);
		template <typename T, typename U> Quat<typename ArithmeticConversion<T, U>::Result> operator /(const Quat<T> &, const Axan<U> &);
		template <typename T, typename U> Quat<T> &operator *=(const Quat<T> &, const Axan<U> &);
		template <typename T, typename U> Quat<T> &operator /=(const Quat<T> &, const Axan<U> &);

		// euler transformation
		template <typename T, typename U> Quat<typename ArithmeticConversion<T, U>::Result> operator *(const Quat<T> &, const Euler<U> &);
		template <typename T, typename U> Quat<typename ArithmeticConversion<T, U>::Result> operator /(const Quat<T> &, const Euler<U> &);
		template <typename T, typename U> Quat<T> &operator *=(const Quat<T> &, const Euler<U> &);
		template <typename T, typename U> Quat<T> &operator /=(const Quat<T> &, const Euler<U> &);

		// matrix transformation
		template <typename T, typename U> Quat<typename ArithmeticConversion<T, U>::Result> operator *(const Quat<T> &, const Matrix<3, 3, U> &);
		template <typename T, typename U> Quat<typename ArithmeticConversion<T, U>::Result> operator /(const Quat<T> &, const Matrix<3, 3, U> &);
		template <typename T, typename U> Quat<T> &operator *=(const Quat<T> &, const Matrix<3, 3, U> &);
		template <typename T, typename U> Quat<T> &operator /=(const Quat<T> &, const Matrix<3, 3, U> &);

		// vector transformation
		template <typename T, typename U> Vector<3, typename ArithmeticConversion<T, U>::Result> operator *(const Quat<T> &, const Vector<3, U> &);

		// stream insertion/extraction
		template <typename T> std::ostream &operator <<(std::ostream &, const Quat<T> &);
		template <typename T> std::istream &operator >>(std::istream &, Quat<T> &);

		// specialized algorithms
		template <typename T> void swap(Quat<T> &, Quat<T> &);
	}
}

#	include "Quat.tpp"
#endif
