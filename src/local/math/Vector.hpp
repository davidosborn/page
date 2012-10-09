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

#ifndef    page_local_math_Vector_hpp
#   define page_local_math_Vector_hpp

#	include <cstddef> // ptrdiff_t, size_t
#	include <functional> // hash
#	include <iosfwd> // [io]stream
#	include <iterator> // reverse_iterator
#	include <type_traits> // enable_if, is_integral
#	include "ArithmeticConversion.hpp"
#	include "float.hpp" // Real
#	include "fwd.hpp" // defaultType, Vector

namespace page
{
	namespace math
	{
		/**
		 * @class BasicVector
		 *
		 * The base class of @c Vector, providing a standard implementation
		 * which all specializations share.
		 */
		template <unsigned n, typename T> struct BasicVector
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
			BasicVector(T = 0);
			BasicVector(const BasicVector &);
			template <unsigned m, typename U> explicit BasicVector(const BasicVector<m, U> &);

			// assignment
			BasicVector &operator =(T);
			BasicVector &operator =(const BasicVector &);

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
			reference operator [](size_type);
			const_reference operator [](size_type) const;
		};

		/**
		 * @class Vector
		 *
		 * A vector class, with specializations for common configurations.
		 */
		template <unsigned n, typename T> struct Vector : BasicVector<n, T>
		{
			typedef BasicVector<n, T> Base;

			// constructors
			Vector(T = 0);
			template <unsigned m, typename U> explicit Vector(const Vector<m, U> &);

			// assignment
			Vector &operator =(T);

			// conversion
			template <typename U> operator Vector<n, U>() const;

			using Base::begin;
			using Base::end;

			T _data[n]; // HACK: conceptually private
		};
		template <typename T> struct Vector<2, T> : BasicVector<2, T>
		{
			typedef BasicVector<2, T> Base;

			// constructors
			Vector(T = 0);
			Vector(T, T);
			template <unsigned m, typename U> explicit Vector(const Vector<m, U> &);

			// assignment
			Vector &operator =(T);

			// conversion
			template <typename U> operator Vector<2, U>() const;

			using Base::begin;
			using Base::end;

			union
			{
				__extension__ struct { T x, y; };
				T _data[2]; // HACK: conceptually private
			};
		};
		template <typename T> struct Vector<3, T> : BasicVector<3, T>
		{
			typedef BasicVector<3, T> Base;

			// constructors
			Vector(T = 0);
			Vector(T, T, T);
			Vector(const Vector<2, T> &, T);
			template <unsigned m, typename U> explicit Vector(const Vector<m, U> &);

			// assignment
			Vector &operator =(T);

			// conversion
			template <typename U> operator Vector<3, U>() const;

			using Base::begin;
			using Base::end;

			union
			{
				__extension__ struct { T x, y, z; };
				T _data[3]; // HACK: conceptually private
			};
		};
		template <typename T> struct Vector<4, T> : BasicVector<4, T>
		{
			typedef BasicVector<4, T> Base;

			// constructors
			Vector(T = 0, T = 1);
			Vector(T, T, T, T = 1);
			Vector(const Vector<2, T> &, T, T = 1);
			Vector(const Vector<3, T> &, T);
			template <unsigned m, typename U> explicit Vector(const Vector<m, U> &);

			// assignment
			Vector &operator =(T);

			// conversion
			template <typename U> operator Vector<4, U>() const;

			using Base::begin;
			using Base::end;

			union
			{
				__extension__ struct { T x, y, z, w; };
				T _data[4]; // HACK: conceptually private
			};
		};

		// initialization
		template <unsigned n, typename T = defaultType> typename std::enable_if<n != 4, Vector<n, T>>::type ZeroVector();
		template <unsigned n, typename T = defaultType> typename std::enable_if<n != 4, Vector<n, T>>::type NormVector();
		template <unsigned n, typename T = defaultType> typename std::enable_if<n != 4, Vector<n, T>>::type ScaleVector();
		template <unsigned n, typename T = defaultType> typename std::enable_if<n == 2 || n == 3, Vector<n, T>>::type UpVector();

		// 4 component initialization
		template <unsigned n, typename T = defaultType> typename std::enable_if<n == 4, Vector<n, T>>::type ZeroVector(T = 1);
		template <unsigned n, typename T = defaultType> typename std::enable_if<n == 4, Vector<n, T>>::type NormVector(T = 1);
		template <unsigned n, typename T = defaultType> typename std::enable_if<n == 4, Vector<n, T>>::type ScaleVector(T = 1);
		template <unsigned n, typename T = defaultType> typename std::enable_if<n == 4, Vector<n, T>>::type UpVector(T = 1);

		// unary operators
		template <unsigned n, typename T> Vector<n, T> &operator ++(Vector<n, T> &);
		template <unsigned n, typename T> Vector<n, T> &operator --(Vector<n, T> &);
		template <unsigned n, typename T> Vector<n, T> operator ++(Vector<n, T> &, int);
		template <unsigned n, typename T> Vector<n, T> operator --(Vector<n, T> &, int);
		template <unsigned n, typename T> Vector<n, T> operator +(const Vector<n, T> &);
		template <unsigned n, typename T> Vector<n, T> operator -(const Vector<n, T> &);

		// arithmetic operators
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator *(const Vector<n, T> &, const Vector<n, U> &);
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator /(const Vector<n, T> &, const Vector<n, U> &);
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator %(const Vector<n, T> &, const Vector<n, U> &);
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator +(const Vector<n, T> &, const Vector<n, U> &);
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator -(const Vector<n, T> &, const Vector<n, U> &);

		// scalar arithmetic operators
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator *(const Vector<n, T> &, U);
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator /(const Vector<n, T> &, U);
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator %(const Vector<n, T> &, U);
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator +(const Vector<n, T> &, U);
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator -(const Vector<n, T> &, U);

		// reverse scalar arithmetic operators
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator *(T, const Vector<n, U> &);
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator /(T, const Vector<n, U> &);
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator %(T, const Vector<n, U> &);
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator +(T, const Vector<n, U> &);
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator -(T, const Vector<n, U> &);

		// shift operators
		template <unsigned n, typename T> Vector<n, T> operator <<(const Vector<n, T> &, const Vector<n, unsigned> &);
		template <unsigned n, typename T> Vector<n, T> operator >>(const Vector<n, T> &, const Vector<n, unsigned> &);
		template <unsigned n, typename T> Vector<n, T> operator <<(const Vector<n, T> &, unsigned);
		template <unsigned n, typename T> Vector<n, T> operator >>(const Vector<n, T> &, unsigned);
		// HACK: using SFINAE to resolve stream insertion/extraction ambiguity
		template <unsigned n, typename T> typename std::enable_if<std::is_integral<T>::value, Vector<n, T>>::type operator <<(T, const Vector<n, unsigned> &);
		template <unsigned n, typename T> typename std::enable_if<std::is_integral<T>::value, Vector<n, T>>::type operator >>(T, const Vector<n, unsigned> &);

		// relational operators
		template <unsigned n, typename T, typename U> Vector<n, bool> operator <(const Vector<n, T> &, const Vector<n, U> &);
		template <unsigned n, typename T, typename U> Vector<n, bool> operator >(const Vector<n, T> &, const Vector<n, U> &);
		template <unsigned n, typename T, typename U> Vector<n, bool> operator <=(const Vector<n, T> &, const Vector<n, U> &);
		template <unsigned n, typename T, typename U> Vector<n, bool> operator >=(const Vector<n, T> &, const Vector<n, U> &);
		template <unsigned n, typename T, typename U> Vector<n, bool> operator ==(const Vector<n, T> &, const Vector<n, U> &);
		template <unsigned n, typename T, typename U> Vector<n, bool> operator !=(const Vector<n, T> &, const Vector<n, U> &);

		// scalar relational operators
		template <unsigned n, typename T, typename U> Vector<n, bool> operator <(const Vector<n, T> &, U);
		template <unsigned n, typename T, typename U> Vector<n, bool> operator >(const Vector<n, T> &, U);
		template <unsigned n, typename T, typename U> Vector<n, bool> operator <=(const Vector<n, T> &, U);
		template <unsigned n, typename T, typename U> Vector<n, bool> operator >=(const Vector<n, T> &, U);
		template <unsigned n, typename T, typename U> Vector<n, bool> operator ==(const Vector<n, T> &, U);
		template <unsigned n, typename T, typename U> Vector<n, bool> operator !=(const Vector<n, T> &, U);

		// reverse scalar relational operators
		template <unsigned n, typename T, typename U> Vector<n, bool> operator <(T, const Vector<n, U> &);
		template <unsigned n, typename T, typename U> Vector<n, bool> operator >(T, const Vector<n, U> &);
		template <unsigned n, typename T, typename U> Vector<n, bool> operator <=(T, const Vector<n, U> &);
		template <unsigned n, typename T, typename U> Vector<n, bool> operator >=(T, const Vector<n, U> &);
		template <unsigned n, typename T, typename U> Vector<n, bool> operator ==(T, const Vector<n, U> &);
		template <unsigned n, typename T, typename U> Vector<n, bool> operator !=(T, const Vector<n, U> &);

		// bitwise operators
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator &(const Vector<n, T> &, const Vector<n, U> &);
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator ^(const Vector<n, T> &, const Vector<n, U> &);
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator |(const Vector<n, T> &, const Vector<n, U> &);
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator &(const Vector<n, T> &, U);
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator ^(const Vector<n, T> &, U);
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator |(const Vector<n, T> &, U);
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator &(T, const Vector<n, U> &);
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator ^(T, const Vector<n, U> &);
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator |(T, const Vector<n, U> &);

		// logical operators
		template <unsigned n, typename T, typename U> Vector<n, bool> operator &&(const Vector<n, T> &, const Vector<n, U> &);
		template <unsigned n, typename T, typename U> Vector<n, bool> operator ||(const Vector<n, T> &, const Vector<n, U> &);
		template <unsigned n, typename T, typename U> Vector<n, bool> operator &&(const Vector<n, T> &, U);
		template <unsigned n, typename T, typename U> Vector<n, bool> operator ||(const Vector<n, T> &, U);
		template <unsigned n, typename T, typename U> Vector<n, bool> operator &&(T, const Vector<n, U> &);
		template <unsigned n, typename T, typename U> Vector<n, bool> operator ||(T, const Vector<n, U> &);
		template <unsigned n, typename T> Vector<n, bool> operator !(const Vector<n, T> &);

		// assignment operators
		template <unsigned n, typename T, typename U> Vector<n, T> &operator *=(Vector<n, T> &, const Vector<n, U> &);
		template <unsigned n, typename T, typename U> Vector<n, T> &operator /=(Vector<n, T> &, const Vector<n, U> &);
		template <unsigned n, typename T, typename U> Vector<n, T> &operator %=(Vector<n, T> &, const Vector<n, U> &);
		template <unsigned n, typename T, typename U> Vector<n, T> &operator +=(Vector<n, T> &, const Vector<n, U> &);
		template <unsigned n, typename T, typename U> Vector<n, T> &operator -=(Vector<n, T> &, const Vector<n, U> &);
		template <unsigned n, typename T> Vector<n, T> &operator <<=(Vector<n, T> &, const Vector<n, unsigned> &);
		template <unsigned n, typename T> Vector<n, T> &operator >>=(Vector<n, T> &, const Vector<n, unsigned> &);
		template <unsigned n, typename T, typename U> Vector<n, T> &operator &=(Vector<n, T> &, const Vector<n, U> &);
		template <unsigned n, typename T, typename U> Vector<n, T> &operator ^=(Vector<n, T> &, const Vector<n, U> &);
		template <unsigned n, typename T, typename U> Vector<n, T> &operator |=(Vector<n, T> &, const Vector<n, U> &);

		// scalar assignment operators
		template <unsigned n, typename T, typename U> Vector<n, T> &operator *=(Vector<n, T> &, U);
		template <unsigned n, typename T, typename U> Vector<n, T> &operator /=(Vector<n, T> &, U);
		template <unsigned n, typename T, typename U> Vector<n, T> &operator %=(Vector<n, T> &, U);
		template <unsigned n, typename T, typename U> Vector<n, T> &operator +=(Vector<n, T> &, U);
		template <unsigned n, typename T, typename U> Vector<n, T> &operator -=(Vector<n, T> &, U);
		template <unsigned n, typename T> Vector<n, T> &operator <<=(Vector<n, T> &, unsigned);
		template <unsigned n, typename T> Vector<n, T> &operator >>=(Vector<n, T> &, unsigned);
		template <unsigned n, typename T, typename U> Vector<n, T> &operator &=(Vector<n, T> &, U);
		template <unsigned n, typename T, typename U> Vector<n, T> &operator ^=(Vector<n, T> &, U);
		template <unsigned n, typename T, typename U> Vector<n, T> &operator |=(Vector<n, T> &, U);

		// boolean combiners
		template <unsigned n, typename T> bool All(const Vector<n, T> &);
		template <unsigned n, typename T> bool Any(const Vector<n, T> &);
		template <unsigned n, typename B, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> Select(const Vector<n, B> &, const Vector<n, T> &, const Vector<n, U> &);
		template <unsigned n, typename B, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> Select(const Vector<n, B> &, const Vector<n, T> &, U);
		template <unsigned n, typename B, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> Select(const Vector<n, B> &, T, const Vector<n, U> &);
		template <unsigned n, typename B, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> Select(const Vector<n, B> &, T, U);

		// min/max
		template <unsigned n, typename T> T Min(const Vector<n, T> &);
		template <unsigned n, typename T> T Max(const Vector<n, T> &);
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> Min(const Vector<n, T> &, const Vector<n, U> &);
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> Min(const Vector<n, T> &, U);
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> Min(T, const Vector<n, U> &);
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> Max(const Vector<n, T> &, const Vector<n, U> &);
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> Max(const Vector<n, T> &, U);
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> Max(T, const Vector<n, U> &);

		// swizzle
		template <unsigned n, typename T> Vector<1, T> Swizzle(const Vector<n, T> &, unsigned);
		template <unsigned n, typename T> Vector<2, T> Swizzle(const Vector<n, T> &, unsigned, unsigned);
		template <unsigned n, typename T> Vector<3, T> Swizzle(const Vector<n, T> &, unsigned, unsigned, unsigned);
		template <unsigned n, typename T> Vector<4, T> Swizzle(const Vector<n, T> &, unsigned, unsigned, unsigned, unsigned);

		// tolerant equivalence
		template <unsigned n, typename T, typename U> Vector<n, bool> Near(const Vector<n, T> &, const Vector<n, U> &);
		template <unsigned n, typename T, typename U> Vector<n, bool> Near(const Vector<n, T> &, U);
		template <unsigned n, typename T, typename U> Vector<n, bool> Near(T, const Vector<n, U> &);

		// quantization
		template <unsigned n, typename T> Vector<n, T> Round(const Vector<n, T> &);
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> Round(const Vector<n, T> &, U precision);
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> Round(const Vector<n, T> &, const Vector<n, U> &precision);

		// other operations
		template <unsigned n, typename T> Vector<n, T> Abs(const Vector<n, T> &);
		template <typename T, typename U> typename ArithmeticConversion<T, U>::Result AngleBetween(const Vector<2, T> &, const Vector<2, U> &);
		template <typename T, typename U> typename ArithmeticConversion<T, U>::Result AngleBetween(const Vector<3, T> &, const Vector<3, U> &);
		template <typename T> typename Real<T>::Type Aspect(const Vector<2, T> &);
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
		template <typename T> Vector<2, T> Tpos(const Vector<2, T> &);

		// coordinate system transformation
		template <typename T> Vector<2, T> Barycentric(const Vector<2, T> &, const Vector<2, T> &, const Vector<2, T> &, const Vector<2, T> &point);

		// stream insertion/extraction
		template <unsigned n, typename T> std::ostream &operator <<(std::ostream &, const Vector<n, T> &);
		template <unsigned n, typename T> std::istream &operator >>(std::istream &, Vector<n, T> &);

		// specialized algorithms
		template <unsigned n, typename T> void swap(Vector<n, T> &, Vector<n, T> &);
	}
}

namespace std
{
	// hash specialization
	template <unsigned n, typename T> struct hash< ::page::math::Vector<n, T>> :
		unary_function< ::page::math::Vector<n, T>, size_t>
	{
		size_t operator ()(const ::page::math::Vector<n, T> &) const;
	};
}

#	include "Vector.tpp"
#endif
