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

#include <algorithm> // copy, fill{,_n}, find, {max,min}{,_element}, swap_ranges, transform
#include <cassert>
#include <cmath> // abs, atan2, ceil, cos, floor, sin, sqrt
#include <cstdlib> // abs
#include <functional> // bind, {bit,logical}_{and,not,or}, divides, {,not_}equal_to, {greater,less}{,_equal}, minus, multiplies, negate, plus
#include <numeric> // accumulate, inner_product
#include "../util/algorithm.hpp" // select, transform_n
#include "../util/functional.hpp" // bit_sh[lr], modulus
#include "../util/hash.hpp" // hash_range
#include "../util/iterator/range.hpp"
#include "../util/serialize/deserialize.hpp" // Deserialize
#include "../util/serialize/serialize.hpp" // Serialize
#include "float.hpp" // Near, Round

namespace page
{
	namespace math
	{
		// basic vector
		// constructors
		template <unsigned n, typename T> BasicVector<n, T>::BasicVector(T t)
		{
			std::fill(begin(), end(), t);
		}
		template <unsigned n, typename T> BasicVector<n, T>::BasicVector(const BasicVector &other)
		{
			std::copy(other.begin(), other.end(), begin());
		}
		template <unsigned n, typename T> template <unsigned m, typename U> BasicVector<n, T>::BasicVector(const BasicVector<m, U> &other)
		{
			std::fill(util::transform_n(other.begin(), std::min(n, m), begin(), [](U t){ return static_cast<T>(t); }), end(), 0);
		}

		// assignment
		template <unsigned n, typename T> BasicVector<n, T> &BasicVector<n, T>::operator =(T t)
		{
			std::fill(begin(), end(), t);
			return *this;
		}
		template <unsigned n, typename T> BasicVector<n, T> &BasicVector<n, T>::operator =(const BasicVector &other)
		{
			std::copy(other.begin(), other.end(), begin());
			return *this;
		}

		// iterators
		template <unsigned n, typename T> typename BasicVector<n, T>::iterator BasicVector<n, T>::begin()
			{ return static_cast<Vector<n, T> &>(*this)._data; }
		template <unsigned n, typename T> typename BasicVector<n, T>::const_iterator BasicVector<n, T>::begin() const
			{ return static_cast<const Vector<n, T> &>(*this)._data; }
		template <unsigned n, typename T> typename BasicVector<n, T>::iterator BasicVector<n, T>::end()
			{ return begin() + n; }
		template <unsigned n, typename T> typename BasicVector<n, T>::const_iterator BasicVector<n, T>::end() const
			{ return begin() + n; }

		// reverse iterators
		template <unsigned n, typename T> typename BasicVector<n, T>::reverse_iterator BasicVector<n, T>::rbegin()
			{ return reverse_iterator(end()); }
		template <unsigned n, typename T> typename BasicVector<n, T>::const_reverse_iterator BasicVector<n, T>::rbegin() const
			{ return const_reverse_iterator(end()); }
		template <unsigned n, typename T> typename BasicVector<n, T>::reverse_iterator BasicVector<n, T>::rend()
			{ return reverse_iterator(begin()); }
		template <unsigned n, typename T> typename BasicVector<n, T>::const_reverse_iterator BasicVector<n, T>::rend() const
			{ return const_reverse_iterator(begin()); }

		// element access
		template <unsigned n, typename T> typename BasicVector<n, T>::reference BasicVector<n, T>::operator [](size_type i)
			{ return begin()[i]; }
		template <unsigned n, typename T> typename BasicVector<n, T>::const_reference BasicVector<n, T>::operator [](size_type i) const
			{ return begin()[i]; }

		// vector
		// constructors
		template <unsigned n, typename T> Vector<n, T>::Vector(T t) :
			Base(t) {}
		template <unsigned n, typename T> template <unsigned m, typename U> Vector<n, T>::Vector(const Vector<m, U> &other) :
			Base(other) {}

		// assignment
		template <unsigned n, typename T> Vector<n, T> &Vector<n, T>::operator =(T t)
		{
			Base::operator =(t);
			return *this;
		}

		// conversion
		template <unsigned n, typename T> template <typename U> Vector<n, T>::operator Vector<n, U>() const
		{
			Vector<n, U> r;
			std::copy(begin(), end(), r.begin());
			return r;
		}

		// 2 component vector
		// constructors
		template <typename T> Vector<2, T>::Vector(T t) :
			Base(t) {}
		template <typename T> Vector<2, T>::Vector(T x, T y) :
			x(x), y(y) {}
		template <typename T> template <unsigned m, typename U> Vector<2, T>::Vector(const Vector<m, U> &other) :
			Base(other) {}

		// assignment
		template <typename T> Vector<2, T> &Vector<2, T>::operator =(T t)
		{
			Base::operator =(t);
			return *this;
		}

		// conversion
		template <typename T> template <typename U> Vector<2, T>::operator Vector<2, U>() const
		{
			Vector<2, U> r;
			std::copy(begin(), end(), r.begin());
			return r;
		}

		// 3 component vector
		// constructors
		template <typename T> Vector<3, T>::Vector(T t) :
			Base(t) {}
		template <typename T> Vector<3, T>::Vector(T x, T y, T z) :
			x(x), y(y), z(z) {}
		template <typename T> Vector<3, T>::Vector(const Vector<2, T> &other, T z) :
			Base(other), z(z) {}
		template <typename T> template <unsigned m, typename U> Vector<3, T>::Vector(const Vector<m, U> &other) :
			Base(other) {}

		// assignment
		template <typename T> Vector<3, T> &Vector<3, T>::operator =(T t)
		{
			Base::operator =(t);
			return *this;
		}

		// conversion
		template <typename T> template <typename U> Vector<3, T>::operator Vector<3, U>() const
		{
			Vector<3, U> r;
			std::copy(begin(), end(), r.begin());
			return r;
		}

		// 4 component vector
		// constructors
		template <typename T> Vector<4, T>::Vector(T t, T w) :
			Base(t), w(w) {}
		template <typename T> Vector<4, T>::Vector(T x, T y, T z, T w) :
			x(x), y(y), z(z), w(w) {}
		template <typename T> Vector<4, T>::Vector(const Vector<2, T> &other, T z, T w) :
			Base(other), z(z), w(w) {}
		template <typename T> Vector<4, T>::Vector(const Vector<3, T> &other, T w) :
			Base(other), w(w) {}
		template <typename T> template <unsigned m, typename U> Vector<4, T>::Vector(const Vector<m, U> &other) :
			Base(other) {}

		// assignment
		template <typename T> Vector<4, T> &Vector<4, T>::operator =(T t)
		{
			Base::operator =(t);
			return *this;
		}

		// conversion
		template <typename T> template <typename U> Vector<4, T>::operator Vector<4, U>() const
		{
			Vector<4, U> r;
			std::copy(begin(), end(), r.begin());
			return r;
		}

		// initialization
		template <unsigned n, typename T> typename std::enable_if<n != 4, Vector<n, T>>::type ZeroVector()
		{
			return Vector<n, T>(0);
		}
		template <unsigned n, typename T> typename std::enable_if<n != 4, Vector<n, T>>::type NormVector()
		{
			Vector<n, T> r;
			r[n - 1] = 1;
			return r;
		}
		template <unsigned n, typename T> typename std::enable_if<n != 4, Vector<n, T>>::type ScaleVector()
		{
			return Vector<n, T>(1);
		}
		template <unsigned n, typename T> typename std::enable_if<n == 2 || n == 3, Vector<n, T>>::type UpVector()
		{
			Vector<n, T> r;
			r[1] = 1;
			return r;
		}

		// 4 component initialization
		template <unsigned n, typename T> typename std::enable_if<n == 4, Vector<n, T>>::type ZeroVector(T w)
		{
			return Vector<n, T>(0, w);
		}
		template <unsigned n, typename T> typename std::enable_if<n == 4, Vector<n, T>>::type NormVector(T w)
		{
			return Vector<n, T>(0, 0, 1, w);
		}
		template <unsigned n, typename T> typename std::enable_if<n == 4, Vector<n, T>>::type ScaleVector(T w)
		{
			return Vector<n, T>(1, w);
		}
		template <unsigned n, typename T> typename std::enable_if<n == 4, Vector<n, T>>::type UpVector(T w)
		{
			return Vector<n, T>(0, 1, 0, w);
		}

		// unary operators
		template <unsigned n, typename T> Vector<n, T> &operator ++(Vector<n, T> &v)
		{
			std::transform(v.begin(), v.end(), v.begin(), std::bind2nd(std::plus<T>(), 1));
			return v;
		}
		template <unsigned n, typename T> Vector<n, T> &operator --(Vector<n, T> &v)
		{
			std::transform(v.begin(), v.end(), v.begin(), std::bind2nd(std::minus<T>(), 1));
			return v;
		}
		template <unsigned n, typename T> Vector<n, T> operator ++(Vector<n, T> &v, int)
		{
			Vector<n, T> r;
			std::transform(v.begin(), v.end(), r.begin(), std::bind2nd(std::plus<T>(), 1));
			return r;
		}
		template <unsigned n, typename T> Vector<n, T> operator --(Vector<n, T> &v, int)
		{
			Vector<n, T> r;
			std::transform(v.begin(), v.end(), r.begin(), std::bind2nd(std::minus<T>(), 1));
			return r;
		}
		template <unsigned n, typename T> Vector<n, T> operator +(const Vector<n, T> &v)
		{
			return v;
		}
		template <unsigned n, typename T> Vector<n, T> operator -(const Vector<n, T> &v)
		{
			Vector<n, T> r;
			std::transform(v.begin(), v.end(), r.begin(), std::negate<T>());
			return r;
		}

		// arithmetic operators
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator *(const Vector<n, T> &v1, const Vector<n, U> &v2)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Vector<n, R> r;
			std::transform(v1.begin(), v1.end(), v2.begin(), r.begin(), std::multiplies<R>());
			return r;
		}
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator /(const Vector<n, T> &v1, const Vector<n, U> &v2)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Vector<n, R> r;
			std::transform(v1.begin(), v1.end(), v2.begin(), r.begin(), std::divides<R>());
			return r;
		}
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator %(const Vector<n, T> &v1, const Vector<n, U> &v2)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Vector<n, R> r;
			std::transform(v1.begin(), v1.end(), v2.begin(), r.begin(), util::modulus<R>());
			return r;
		}
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator +(const Vector<n, T> &v1, const Vector<n, U> &v2)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Vector<n, R> r;
			std::transform(v1.begin(), v1.end(), v2.begin(), r.begin(), std::plus<R>());
			return r;
		}
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator -(const Vector<n, T> &v1, const Vector<n, U> &v2)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Vector<n, R> r;
			std::transform(v1.begin(), v1.end(), v2.begin(), r.begin(), std::minus<R>());
			return r;
		}

		// scalar arithmetic operators
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator *(const Vector<n, T> &v, U t)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Vector<n, R> r;
			std::transform(v.begin(), v.end(), r.begin(), std::bind(std::multiplies<R>(), std::placeholders::_1, t));
			return r;
		}
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator /(const Vector<n, T> &v, U t)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Vector<n, R> r;
			std::transform(v.begin(), v.end(), r.begin(), std::bind(std::divides<R>(), std::placeholders::_1, t));
			return r;
		}
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator %(const Vector<n, T> &v, U t)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Vector<n, R> r;
			std::transform(v.begin(), v.end(), r.begin(), std::bind(util::modulus<R>(), std::placeholders::_1, t));
			return r;
		}
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator +(const Vector<n, T> &v, U t)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Vector<n, R> r;
			std::transform(v.begin(), v.end(), r.begin(), std::bind(std::plus<R>(), std::placeholders::_1, t));
			return r;
		}
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator -(const Vector<n, T> &v, U t)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Vector<n, R> r;
			std::transform(v.begin(), v.end(), r.begin(), std::bind(std::minus<R>(), std::placeholders::_1, t));
			return r;
		}

		// reverse scalar arithmetic operators
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator *(T t, const Vector<n, U> &v)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Vector<n, R> r;
			std::transform(v.begin(), v.end(), r.begin(), std::bind(std::multiplies<R>(), t, std::placeholders::_1));
			return r;
		}
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator /(T t, const Vector<n, U> &v)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Vector<n, R> r;
			std::transform(v.begin(), v.end(), r.begin(), std::bind(std::divides<R>(), t, std::placeholders::_1));
			return r;
		}
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator %(T t, const Vector<n, U> &v)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Vector<n, R> r;
			std::transform(v.begin(), v.end(), r.begin(), std::bind(util::modulus<R>(), t, std::placeholders::_1));
			return r;
		}
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator +(T t, const Vector<n, U> &v)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Vector<n, R> r;
			std::transform(v.begin(), v.end(), r.begin(), std::bind(std::plus<R>(), t, std::placeholders::_1));
			return r;
		}
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator -(T t, const Vector<n, U> &v)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Vector<n, R> r;
			std::transform(v.begin(), v.end(), r.begin(), std::bind(std::minus<R>(), t, std::placeholders::_1));
			return r;
		}

		// shift operators
		template <unsigned n, typename T> Vector<n, T> operator <<(const Vector<n, T> &v1, const Vector<n, unsigned> &v2)
		{
			Vector<n, T> r;
			std::transform(v1.begin(), v1.end(), v2.begin(), r.begin(), util::bit_shl<T>());
			return r;
		}
		template <unsigned n, typename T> Vector<n, T> operator >>(const Vector<n, T> &v1, const Vector<n, unsigned> &v2)
		{
			Vector<n, T> r;
			std::transform(v1.begin(), v1.end(), v2.begin(), r.begin(), util::bit_shr<T>());
			return r;
		}
		template <unsigned n, typename T> Vector<n, T> operator <<(const Vector<n, T> &v, unsigned t)
		{
			Vector<n, T> r;
			std::transform(v.begin(), v.end(), r.begin(), std::bind(util::bit_shl<T>(), std::placeholders::_1, t));
			return r;
		}
		template <unsigned n, typename T> Vector<n, T> operator >>(const Vector<n, T> &v, unsigned t)
		{
			Vector<n, T> r;
			std::transform(v.begin(), v.end(), r.begin(), std::bind(util::bit_shr<T>(), std::placeholders::_1, t));
			return r;
		}
		template <unsigned n, typename T> typename std::enable_if<std::is_integral<T>::value, Vector<n, T>>::type operator <<(T t, const Vector<n, unsigned> &v)
		{
			Vector<n, T> r;
			std::transform(v.begin(), v.end(), r.begin(), std::bind(util::bit_shl<T>(), t, std::placeholders::_1));
			return r;
		}
		template <unsigned n, typename T> typename std::enable_if<std::is_integral<T>::value, Vector<n, T>>::type operator >>(T t, const Vector<n, unsigned> &v)
		{
			Vector<n, T> r;
			std::transform(v.begin(), v.end(), r.begin(), std::bind(util::bit_shr<T>(), t, std::placeholders::_1));
			return r;
		}

		// relational operators
		template <unsigned n, typename T, typename U> Vector<n, bool> operator <(const Vector<n, T> &v1, const Vector<n, U> &v2)
		{
			Vector<n, bool> r;
			std::transform(v1.begin(), v1.end(), v2.begin(), r.begin(), std::less<typename ArithmeticConversion<T, U>::Result>());
			return r;
		}
		template <unsigned n, typename T, typename U> Vector<n, bool> operator >(const Vector<n, T> &v1, const Vector<n, U> &v2)
		{
			Vector<n, bool> r;
			std::transform(v1.begin(), v1.end(), v2.begin(), r.begin(), std::greater<typename ArithmeticConversion<T, U>::Result>());
			return r;
		}
		template <unsigned n, typename T, typename U> Vector<n, bool> operator <=(const Vector<n, T> &v1, const Vector<n, U> &v2)
		{
			Vector<n, bool> r;
			std::transform(v1.begin(), v1.end(), v2.begin(), r.begin(), std::less_equal<typename ArithmeticConversion<T, U>::Result>());
			return r;
		}
		template <unsigned n, typename T, typename U> Vector<n, bool> operator >=(const Vector<n, T> &v1, const Vector<n, U> &v2)
		{
			Vector<n, bool> r;
			std::transform(v1.begin(), v1.end(), v2.begin(), r.begin(), std::greater_equal<typename ArithmeticConversion<T, U>::Result>());
			return r;
		}
		template <unsigned n, typename T, typename U> Vector<n, bool> operator ==(const Vector<n, T> &v1, const Vector<n, U> &v2)
		{
			Vector<n, bool> r;
			std::transform(v1.begin(), v1.end(), v2.begin(), r.begin(), std::equal_to<typename ArithmeticConversion<T, U>::Result>());
			return r;
		}
		template <unsigned n, typename T, typename U> Vector<n, bool> operator !=(const Vector<n, T> &v1, const Vector<n, U> &v2)
		{
			Vector<n, bool> r;
			std::transform(v1.begin(), v1.end(), v2.begin(), r.begin(), std::not_equal_to<typename ArithmeticConversion<T, U>::Result>());
			return r;
		}

		// scalar relational operators
		template <unsigned n, typename T, typename U> Vector<n, bool> operator <(const Vector<n, T> &v, U t)
		{
			Vector<n, bool> r;
			std::transform(v.begin(), v.end(), r.begin(), std::bind(std::less<typename ArithmeticConversion<T, U>::Result>(), std::placeholders::_1, t));
			return r;
		}
		template <unsigned n, typename T, typename U> Vector<n, bool> operator >(const Vector<n, T> &v, U t)
		{
			Vector<n, bool> r;
			std::transform(v.begin(), v.end(), r.begin(), std::bind(std::greater<typename ArithmeticConversion<T, U>::Result>(), std::placeholders::_1, t));
			return r;
		}
		template <unsigned n, typename T, typename U> Vector<n, bool> operator <=(const Vector<n, T> &v, U t)
		{
			Vector<n, bool> r;
			std::transform(v.begin(), v.end(), r.begin(), std::bind(std::less_equal<typename ArithmeticConversion<T, U>::Result>(), std::placeholders::_1, t));
			return r;
		}
		template <unsigned n, typename T, typename U> Vector<n, bool> operator >=(const Vector<n, T> &v, U t)
		{
			Vector<n, bool> r;
			std::transform(v.begin(), v.end(), r.begin(), std::bind(std::greater_equal<typename ArithmeticConversion<T, U>::Result>(), std::placeholders::_1, t));
			return r;
		}
		template <unsigned n, typename T, typename U> Vector<n, bool> operator ==(const Vector<n, T> &v, U t)
		{
			Vector<n, bool> r;
			std::transform(v.begin(), v.end(), r.begin(), std::bind(std::equal_to<typename ArithmeticConversion<T, U>::Result>(), std::placeholders::_1, t));
			return r;
		}
		template <unsigned n, typename T, typename U> Vector<n, bool> operator !=(const Vector<n, T> &v, U t)
		{
			Vector<n, bool> r;
			std::transform(v.begin(), v.end(), r.begin(), std::bind(std::not_equal_to<typename ArithmeticConversion<T, U>::Result>(), std::placeholders::_1, t));
			return r;
		}

		// reverse scalar relational operators
		template <unsigned n, typename T, typename U> Vector<n, bool> operator <(T t, const Vector<n, U> &v)
		{
			Vector<n, bool> r;
			std::transform(v.begin(), v.end(), r.begin(), std::bind(std::less<typename ArithmeticConversion<T, U>::Result>(), t, std::placeholders::_1));
			return r;
		}
		template <unsigned n, typename T, typename U> Vector<n, bool> operator >(T t, const Vector<n, U> &v)
		{
			Vector<n, bool> r;
			std::transform(v.begin(), v.end(), r.begin(), std::bind(std::greater<typename ArithmeticConversion<T, U>::Result>(), t, std::placeholders::_1));
			return r;
		}
		template <unsigned n, typename T, typename U> Vector<n, bool> operator <=(T t, const Vector<n, U> &v)
		{
			Vector<n, bool> r;
			std::transform(v.begin(), v.end(), r.begin(), std::bind(std::less_equal<typename ArithmeticConversion<T, U>::Result>(), t, std::placeholders::_1));
			return r;
		}
		template <unsigned n, typename T, typename U> Vector<n, bool> operator >=(T t, const Vector<n, U> &v)
		{
			Vector<n, bool> r;
			std::transform(v.begin(), v.end(), r.begin(), std::bind(std::greater_equal<typename ArithmeticConversion<T, U>::Result>(), t, std::placeholders::_1));
			return r;
		}
		template <unsigned n, typename T, typename U> Vector<n, bool> operator ==(T t, const Vector<n, U> &v)
		{
			Vector<n, bool> r;
			std::transform(v.begin(), v.end(), r.begin(), std::bind(std::equal_to<typename ArithmeticConversion<T, U>::Result>(), t, std::placeholders::_1));
			return r;
		}
		template <unsigned n, typename T, typename U> Vector<n, bool> operator !=(T t, const Vector<n, U> &v)
		{
			Vector<n, bool> r;
			std::transform(v.begin(), v.end(), r.begin(), std::bind(std::not_equal_to<typename ArithmeticConversion<T, U>::Result>(), t, std::placeholders::_1));
			return r;
		}

		// bitwise operators
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator &(const Vector<n, T> &v1, const Vector<n, U> &v2)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Vector<n, R> r;
			std::transform(v1.begin(), v1.end(), v2.begin(), r.begin(), std::bit_and<R>());
			return r;
		}
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator ^(const Vector<n, T> &v1, const Vector<n, U> &v2)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Vector<n, R> r;
			std::transform(v1.begin(), v1.end(), v2.begin(), r.begin(), std::bit_xor<R>());
			return r;
		}
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator |(const Vector<n, T> &v1, const Vector<n, U> &v2)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Vector<n, R> r;
			std::transform(v1.begin(), v1.end(), v2.begin(), r.begin(), std::bit_or<R>());
			return r;
		}
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator &(const Vector<n, T> &v, U t)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Vector<n, R> r;
			std::transform(v.begin(), v.end(), r.begin(), std::bind(std::bit_and<R>(), std::placeholders::_1, t));
			return r;
		}
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator ^(const Vector<n, T> &v, U t)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Vector<n, R> r;
			std::transform(v.begin(), v.end(), r.begin(), std::bind(std::bit_xor<R>(), std::placeholders::_1, t));
			return r;
		}
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator |(const Vector<n, T> &v, U t)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Vector<n, R> r;
			std::transform(v.begin(), v.end(), r.begin(), std::bind(std::bit_or<R>(), std::placeholders::_1, t));
			return r;
		}
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator &(T t, const Vector<n, U> &v)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Vector<n, R> r;
			std::transform(v.begin(), v.end(), r.begin(), std::bind(std::bit_and<R>(), t, std::placeholders::_1));
			return r;
		}
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator ^(T t, const Vector<n, U> &v)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Vector<n, R> r;
			std::transform(v.begin(), v.end(), r.begin(), std::bind(std::bit_xor<R>(), t, std::placeholders::_1));
			return r;
		}
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> operator |(T t, const Vector<n, U> &v)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Vector<n, R> r;
			std::transform(v.begin(), v.end(), r.begin(), std::bind(std::bit_or<R>(), t, std::placeholders::_1));
			return r;
		}

		// logical operators
		template <unsigned n, typename T, typename U> Vector<n, bool> operator &&(const Vector<n, T> &v1, const Vector<n, U> &v2)
		{
			Vector<n, bool> r;
			std::transform(v1.begin(), v1.end(), v2.begin(), r.begin(), std::logical_and<bool>());
			return r;
		}
		template <unsigned n, typename T, typename U> Vector<n, bool> operator ||(const Vector<n, T> &v1, const Vector<n, U> &v2)
		{
			Vector<n, bool> r;
			std::transform(v1.begin(), v1.end(), v2.begin(), r.begin(), std::logical_or<bool>());
			return r;
		}
		template <unsigned n, typename T, typename U> Vector<n, bool> operator &&(const Vector<n, T> &v, U t)
		{
			Vector<n, bool> r;
			std::transform(v.begin(), v.end(), r.begin(), std::bind(std::logical_and<bool>(), std::placeholders::_1, t));
			return r;
		}
		template <unsigned n, typename T, typename U> Vector<n, bool> operator ||(const Vector<n, T> &v, U t)
		{
			Vector<n, bool> r;
			std::transform(v.begin(), v.end(), r.begin(), std::bind(std::logical_or<bool>(), std::placeholders::_1, t));
			return r;
		}
		template <unsigned n, typename T, typename U> Vector<n, bool> operator &&(T t, const Vector<n, U> &v)
		{
			Vector<n, bool> r;
			std::transform(v.begin(), v.end(), r.begin(), std::bind(std::logical_and<bool>(), t, std::placeholders::_1));
			return r;
		}
		template <unsigned n, typename T, typename U> Vector<n, bool> operator ||(T t, const Vector<n, U> &v)
		{
			Vector<n, bool> r;
			std::transform(v.begin(), v.end(), r.begin(), std::bind(std::logical_or<bool>(), t, std::placeholders::_1));
			return r;
		}
		template <unsigned n, typename T> Vector<n, bool> operator !(const Vector<n, T> &v)
		{
			Vector<n, bool> r;
			std::transform(v.begin(), v.end(), r.begin(), std::logical_not<bool>());
			return r;
		}

		// assignment operators
		template <unsigned n, typename T, typename U> Vector<n, T> &operator *=(Vector<n, T> &v1, const Vector<n, U> &v2)
		{
			std::transform(v1.begin(), v1.end(), v2.begin(), v1.begin(), std::multiplies<T>());
			return v1;
		}
		template <unsigned n, typename T, typename U> Vector<n, T> &operator /=(Vector<n, T> &v1, const Vector<n, U> &v2)
		{
			std::transform(v1.begin(), v1.end(), v2.begin(), v1.begin(), std::divides<T>());
			return v1;
		}
		template <unsigned n, typename T, typename U> Vector<n, T> &operator %=(Vector<n, T> &v1, const Vector<n, U> &v2)
		{
			std::transform(v1.begin(), v1.end(), v2.begin(), v1.begin(), util::modulus<T>());
			return v1;
		}
		template <unsigned n, typename T, typename U> Vector<n, T> &operator +=(Vector<n, T> &v1, const Vector<n, U> &v2)
		{
			std::transform(v1.begin(), v1.end(), v2.begin(), v1.begin(), std::plus<T>());
			return v1;
		}
		template <unsigned n, typename T, typename U> Vector<n, T> &operator -=(Vector<n, T> &v1, const Vector<n, U> &v2)
		{
			std::transform(v1.begin(), v1.end(), v2.begin(), v1.begin(), std::minus<T>());
			return v1;
		}
		template <unsigned n, typename T> Vector<n, T> &operator <<=(Vector<n, T> &v1, const Vector<n, unsigned> &v2)
		{
			std::transform(v1.begin(), v1.end(), v2.begin(), v1.begin(), util::bit_shl<T>());
			return v1;
		}
		template <unsigned n, typename T> Vector<n, T> &operator >>=(Vector<n, T> &v1, const Vector<n, unsigned> &v2)
		{
			std::transform(v1.begin(), v1.end(), v2.begin(), v1.begin(), util::bit_shr<T>());
			return v1;
		}
		template <unsigned n, typename T, typename U> Vector<n, T> &operator &=(Vector<n, T> &v1, const Vector<n, U> &v2)
		{
			std::transform(v1.begin(), v1.end(), v2.begin(), v1.begin(), std::bit_and<T>());
			return v1;
		}
		template <unsigned n, typename T, typename U> Vector<n, T> &operator ^=(Vector<n, T> &v1, const Vector<n, U> &v2)
		{
			std::transform(v1.begin(), v1.end(), v2.begin(), v1.begin(), std::bit_xor<T>());
			return v1;
		}
		template <unsigned n, typename T, typename U> Vector<n, T> &operator |=(Vector<n, T> &v1, const Vector<n, U> &v2)
		{
			std::transform(v1.begin(), v1.end(), v2.begin(), v1.begin(), std::bit_or<T>());
			return v1;
		}

		// scalar assignment operators
		template <unsigned n, typename T, typename U> Vector<n, T> &operator *=(Vector<n, T> &v, U t)
		{
			std::transform(v.begin(), v.end(), v.begin(), std::bind(std::multiplies<T>(), std::placeholders::_1, t));
			return v;
		}
		template <unsigned n, typename T, typename U> Vector<n, T> &operator /=(Vector<n, T> &v, U t)
		{
			std::transform(v.begin(), v.end(), v.begin(), std::bind(std::divides<T>(), std::placeholders::_1, t));
			return v;
		}
		template <unsigned n, typename T, typename U> Vector<n, T> &operator %=(Vector<n, T> &v, U t)
		{
			std::transform(v.begin(), v.end(), v.begin(), std::bind(util::modulus<T>(), std::placeholders::_1, t));
			return v;
		}
		template <unsigned n, typename T, typename U> Vector<n, T> &operator +=(Vector<n, T> &v, U t)
		{
			std::transform(v.begin(), v.end(), v.begin(), std::bind(std::plus<T>(), std::placeholders::_1, t));
			return v;
		}
		template <unsigned n, typename T, typename U> Vector<n, T> &operator -=(Vector<n, T> &v, U t)
		{
			std::transform(v.begin(), v.end(), v.begin(), std::bind(std::minus<T>(), std::placeholders::_1, t));
			return v;
		}
		template <unsigned n, typename T> Vector<n, T> &operator <<=(Vector<n, T> &v, unsigned t)
		{
			std::transform(v.begin(), v.end(), v.begin(), std::bind(util::bit_shl<T>(), std::placeholders::_1, t));
			return v;
		}
		template <unsigned n, typename T> Vector<n, T> &operator >>=(Vector<n, T> &v, unsigned t)
		{
			std::transform(v.begin(), v.end(), v.begin(), std::bind(util::bit_shr<T>(), std::placeholders::_1, t));
			return v;
		}
		template <unsigned n, typename T, typename U> Vector<n, T> &operator &=(Vector<n, T> &v, U t)
		{
			std::transform(v.begin(), v.end(), v.begin(), std::bind(std::bit_and<T>(), std::placeholders::_1, t));
			return v;
		}
		template <unsigned n, typename T, typename U> Vector<n, T> &operator ^=(Vector<n, T> &v, U t)
		{
			std::transform(v.begin(), v.end(), v.begin(), std::bind(std::bit_xor<T>(), std::placeholders::_1, t));
			return v;
		}
		template <unsigned n, typename T, typename U> Vector<n, T> &operator |=(Vector<n, T> &v, U t)
		{
			std::transform(v.begin(), v.end(), v.begin(), std::bind(std::bit_or<T>(), std::placeholders::_1, t));
			return v;
		}

		// boolean combiners
		template <unsigned n, typename T> bool All(const Vector<n, T> &v)
		{
			Vector<n, bool> b(v);
			return std::find(b.begin(), b.end(), false) == b.end();
		}
		template <unsigned n, typename T> bool Any(const Vector<n, T> &v)
		{
			Vector<n, bool> b(v);
			return std::find(b.begin(), b.end(), true) != b.end();
		}
		template <unsigned n, typename B, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> Select(const Vector<n, B> &b, const Vector<n, T> &v1, const Vector<n, U> &v2)
		{
			Vector<n, typename ArithmeticConversion<T, U>::Result> r;
			util::select(b.begin(), b.end(), v1.begin(), v2.begin(), r.begin());
			return r;
		}
		template <unsigned n, typename B, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> Select(const Vector<n, B> &b, const Vector<n, T> &v, U t)
		{
			return Select(b, v, Vector<n, U>(t));
		}
		template <unsigned n, typename B, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> Select(const Vector<n, B> &b, T t, const Vector<n, U> &v)
		{
			return Select(b, Vector<n, T>(t), v);
		}
		template <unsigned n, typename B, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> Select(const Vector<n, B> &b, T t1, U t2)
		{
			return Select(b, Vector<n, T>(t1), Vector<n, U>(t2));
		}

		// min/max
		template <unsigned n, typename T> T Min(const Vector<n, T> &v)
		{
			return *std::min_element(v.begin(), v.end());
		}
		template <unsigned n, typename T> T Max(const Vector<n, T> &v)
		{
			return *std::max_element(v.begin(), v.end());
		}
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> Min(const Vector<n, T> &v1, const Vector<n, U> &v2)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Vector<n, R> r;
			std::transform(v1.begin(), v1.end(), v2.begin(), r.begin(), [](T a, U b){ return std::min<R>(a, b); });
			return r;
		}
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> Min(const Vector<n, T> &v, U t)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Vector<n, R> r;
			std::transform(v.begin(), v.end(), r.begin(), [&t](T v){ return std::min<R>(v, t); });
			return r;
		}
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> Min(T t, const Vector<n, U> &v)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Vector<n, R> r;
			std::transform(v.begin(), v.end(), r.begin(), [&t](U v){ return std::min<R>(t, v); });
			return r;
		}
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> Max(const Vector<n, T> &v1, const Vector<n, U> &v2)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Vector<n, R> r;
			std::transform(v1.begin(), v1.end(), v2.begin(), r.begin(), [](T a, U b){ return std::max<R>(a, b); });
			return r;
		}
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> Max(const Vector<n, T> &v, U t)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Vector<n, R> r;
			std::transform(v.begin(), v.end(), r.begin(), [&t](T v){ return std::max<R>(v, t); });
			return r;
		}
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> Max(T t, const Vector<n, U> &v)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Vector<n, R> r;
			std::transform(v.begin(), v.end(), r.begin(), [&t](U v){ return std::max<R>(t, v); });
			return r;
		}

		// swizzle
		template <unsigned n, typename T> Vector<1, T> Swizzle(const Vector<n, T> &v, unsigned i)
		{
			assert(i < n);
			return Vector<1, T>(v[i]);
		}
		template <unsigned n, typename T> Vector<2, T> Swizzle(const Vector<n, T> &v, unsigned i1, unsigned i2)
		{
			assert(i1 < n && i2 < n);
			return Vector<2, T>(v[i1], v[i2]);
		}
		template <unsigned n, typename T> Vector<3, T> Swizzle(const Vector<n, T> &v, unsigned i1, unsigned i2, unsigned i3)
		{
			assert(i1 < n && i2 < n && i3 < n);
			return Vector<3, T>(v[i1], v[i2], v[i3]);
		}
		template <unsigned n, typename T> Vector<4, T> Swizzle(const Vector<n, T> &v, unsigned i1, unsigned i2, unsigned i3, unsigned i4)
		{
			assert(i1 < n && i2 < n && i3 < n && i4 < n);
			return Vector<4, T>(v[i1], v[i2], v[i3], v[i4]);
		}

		// tolerant equivalence
		template <unsigned n, typename T, typename U> Vector<n, bool> Near(const Vector<n, T> &v1, const Vector<n, U> &v2)
		{
			Vector<n, bool> r;
			std::transform(v1.begin(), v1.end(), v2.begin(), r.begin(), [](T v1, U v2){ return Near(v1, v2); });
			return r;
		}
		template <unsigned n, typename T, typename U> Vector<n, bool> Near(const Vector<n, T> &v, U t)
		{
			Vector<n, bool> r;
			std::transform(v.begin(), v.end(), r.begin(), [&t](T v){ return Near(v, t); });
			return r;
		}
		template <unsigned n, typename T, typename U> Vector<n, bool> Near(T t, const Vector<n, U> &v)
		{
			Vector<n, bool> r;
			std::transform(v.begin(), v.end(), r.begin(), [&t](U v){ return Near(t, v); });
			return r;
		}

		// quantization
		template <unsigned n, typename T> Vector<n, T> Round(const Vector<n, T> &v)
		{
			Vector<n, T> r;
			std::transform(v.begin(), v.end(), r.begin(), [](T v){ return Round(v); });
			return r;
		}
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> Round(const Vector<n, T> &v, U precision)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Vector<n, R> r;
			std::transform(v.begin(), v.end(), r.begin(), [&precision](T v){ return Round(v, precision); });
			return r;
		}
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> Round(const Vector<n, T> &v, const Vector<n, U> &precision)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Vector<n, R> r;
			std::transform(v.begin(), v.end(), precision.begin(), r.begin(), [](T v, U precision){ return Round(v, precision); });
			return r;
		}

		// other operations
		template <unsigned n, typename T> Vector<n, T> Abs(const Vector<n, T> &v)
		{
			Vector<n, T> r;
			std::transform(v.begin(), v.end(), r.begin(), [](T t){ return std::abs(t); });
			return r;
		}
		template <typename T, typename U> typename ArithmeticConversion<T, U>::Result AngleBetween(const Vector<2, T> &v1, const Vector<2, U> &v2)
		{
			return std::atan2(PerpDot(v1, v2), Dot(v1, v2));
		}
		template <typename T, typename U> typename ArithmeticConversion<T, U>::Result AngleBetween(const Vector<3, T> &v1, const Vector<3, U> &v2)
		{
			// algorithm courtesy of Jesse Anders
			// http://www.gamedev.net/community/forums/topic.asp?topic_id=359053&#2357351
			return std::atan2(Len(Cross(v1, v2)), Dot(v1, v2));
		}
		template <typename T> typename Real<T>::Type Aspect(const Vector<2, T> &v)
		{
			typedef typename Real<T>::Type R;
			return R(v.x) / v.y;
		}
		template <unsigned n, typename T> Vector<n, T> Ceil(const Vector<n, T> &v)
		{
			Vector<n, T> r;
			std::transform(v.begin(), v.end(), r.begin(), [](T t){ return std::ceil(t); });
			return r;
		}
		template <unsigned n, typename T> T Content(const Vector<n, T> &v)
		{
			return std::accumulate(v.begin(), v.end(), 1, std::multiplies<T>());
		}
		template <typename T> Vector<2, T> Cross(const Vector<2, T> &v)
		{
			return Vector<2, T>(-v.y, v.x);
		}
		template <typename T, typename U> Vector<3, typename ArithmeticConversion<T, U>::Result> Cross(const Vector<3, T> &v1, const Vector<3, U> &v2)
		{
			return
				Swizzle(v1, 1, 2, 0) * Swizzle(v2, 2, 0, 1) -
				Swizzle(v1, 2, 0, 1) * Swizzle(v2, 1, 2, 0);
		}
		template <unsigned n, typename T, typename U> typename ArithmeticConversion<T, U>::Result Dot(const Vector<n, T> &v1, const Vector<n, U> &v2)
		{
			return std::inner_product(v1.begin(), v1.end(), v2.begin(), typename ArithmeticConversion<T, U>::Result());
		}
		template <unsigned n, typename T> Vector<n, T> Floor(const Vector<n, T> &v)
		{
			Vector<n, T> r;
			std::transform(v.begin(), v.end(), r.begin(), [](T t){ return std::floor(t); });
			return r;
		}
		template <unsigned n, typename T> T Len(const Vector<n, T> &v)
		{
			return std::sqrt(Dot(v, v));
		}
		template <unsigned n, typename T> Vector<n, T> Norm(const Vector<n, T> &v)
		{
			assert(Len(v));
			return v / Len(v);
		}
		template <typename T, typename U> typename ArithmeticConversion<T, U>::Result PerpDot(const Vector<2, T> &v1, const Vector<2, U> &v2)
		{
			return Dot(v1, Cross(v2));
		}
		template <unsigned n, typename T, typename U> Vector<n, typename ArithmeticConversion<T, U>::Result> Proj(const Vector<n, T> &a, const Vector<n, U> &b)
		{
			return b * Dot(a, b) / Dot(b, b);
		}
		template <typename T, typename U> Vector<2, typename ArithmeticConversion<T, U>::Result> Rotate(const Vector<2, T> &v, U a)
		{
			return Vector<2, typename ArithmeticConversion<T, U>::Result>(
				 std::cos(a) * v.x + std::sin(a) * v.y,
				-std::sin(a) * v.x + std::cos(a) * v.y);
		}
		template <unsigned n, typename T> T Sum(const Vector<n, T> &v)
		{
			return std::accumulate(v.begin(), v.end(), T(0));
		}
		template <typename T> Vector<2, T> Tpos(const Vector<2, T> &v)
		{
			return Vector<2, T>(v.x, v.y);
		}

		// coordinate system transformation
		// Real-Time Collision Detection, Christer Ericson, 2005
		template <typename T> Vector<2, T> Barycentric(const Vector<2, T> &a, const Vector<2, T> &b, const Vector<2, T> &c, const Vector<2, T> &p)
		{
			Vector<2, T> ab(b - a), ac(c - a), ap(p - a);
			T
				dbb = Dot(ab, ab), dbc = Dot(ab, ac), dcc = Dot(ac, ac),
				dpb = Dot(ap, ab), dpc = Dot(ap, ac), denom = dbb * dcc - dbc * dbc;
			return Vector<2, T>(
				(dcc * dpb - dbc * dpc) / denom,
				(dbb * dpc - dbc * dpb) / denom);
		}

		// stream insertion/extraction
		template <unsigned n, typename T> std::ostream &operator <<(std::ostream &os, const Vector<n, T> &v)
		{
			return util::Serialize(os, util::make_range(v), ',');
		}
		template <unsigned n, typename T> std::istream &operator >>(std::istream &is, Vector<n, T> &v)
		{
			return util::Deserialize(is, util::make_range(v), util::SequenceDeserializationFlags::none, ',');
		}

		// specialized algorithms
		template <unsigned n, typename T> void swap(Vector<n, T> &a, Vector<n, T> &b)
		{
			std::swap_ranges(a.begin(), a.end(), b.begin());
		}
	}
}

namespace std
{
	// hash specialization
	template <unsigned n, typename T> size_t hash< ::page::math::Vector<n, T>>::operator ()(const ::page::math::Vector<n, T> &v) const
	{
		return ::page::util::hash_range(v.begin(), v.end());
	}
}
