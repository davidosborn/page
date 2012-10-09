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

#include <algorithm> // copy, transform
#include <functional> // bind2nd, divides, minus, multiplies, plus, negate
#include "../util/functional.hpp" // modulus
#include "../util/iterator/range.hpp"
#include "../util/serialize/deserialize.hpp" // Deserialize
#include "../util/serialize/serialize.hpp" // Serialize

namespace page
{
	namespace math
	{
		// basic slice
		// constructors
		template <unsigned n, typename Iterator> BasicSlice<n, Iterator>::BasicSlice(const Iterator &iter) :
			iter(iter) {}
		template <unsigned n, typename Iterator> template <typename Iterator2> BasicSlice<n, Iterator>::BasicSlice(const BasicSlice<n, Iterator2> &other) :
			iter(other.iter) {}

		// assignment
		template <unsigned n, typename Iterator> const BasicSlice<n, Iterator> &BasicSlice<n, Iterator>::operator =(const BasicSlice &other) const
		{
			std::copy(other.begin(), other.end(), begin());
			return *this;
		}
		template <unsigned n, typename Iterator> template <typename Iterator2> const BasicSlice<n, Iterator> &BasicSlice<n, Iterator>::operator =(const BasicSlice<n, Iterator2> &other) const
		{
			std::copy(other.begin(), other.end(), begin());
			return *this;
		}
		template <unsigned n, typename Iterator> const BasicSlice<n, Iterator> &BasicSlice<n, Iterator>::operator =(const value_type &t) const
		{
			std::fill(begin(), end(), t);
			return *this;
		}
		template <unsigned n, typename Iterator> const BasicSlice<n, Iterator> &BasicSlice<n, Iterator>::operator =(const Vector<n, value_type> &v) const
		{
			std::copy(v.begin(), v.end(), begin());
			return *this;
		}

		// conversion
		template <unsigned n, typename Iterator> BasicSlice<n, Iterator>::operator Vector<n, value_type>() const
		{
			Vector<n, value_type> r;
			std::copy(begin(), end(), r.begin());
			return r;
		}

		// iterators
		template <unsigned n, typename Iterator> typename BasicSlice<n, Iterator>::iterator BasicSlice<n, Iterator>::begin() const
			{ return iter; }
		template <unsigned n, typename Iterator> typename BasicSlice<n, Iterator>::iterator BasicSlice<n, Iterator>::end() const
			{ return begin() + n; }

		// reverse iterators
		template <unsigned n, typename Iterator> typename BasicSlice<n, Iterator>::reverse_iterator BasicSlice<n, Iterator>::rbegin() const
			{ return reverse_iterator(end()); }
		template <unsigned n, typename Iterator> typename BasicSlice<n, Iterator>::reverse_iterator BasicSlice<n, Iterator>::rend() const
			{ return reverse_iterator(begin()); }

		// element access
		template <unsigned n, typename Iterator> typename BasicSlice<n, Iterator>::reference BasicSlice<n, Iterator>::operator [](size_type i) const
			{ return begin()[i]; }

		// slice
		// constructors
		template <unsigned n, typename Iterator> Slice<n, Iterator>::Slice(const Iterator &iter) :
			Base(iter) {}
		template <unsigned n, typename Iterator> template <typename Iterator2> Slice<n, Iterator>::Slice(const Slice<n, Iterator2> &other) :
			Base(other) {}

		// assignment
		template <unsigned n, typename Iterator> template <typename Iterator2> const Slice<n, Iterator> &Slice<n, Iterator>::operator =(const Slice<n, Iterator2> &other) const
		{
			Base::operator =(other);
			return *this;
		}
		template <unsigned n, typename Iterator> const Slice<n, Iterator> &Slice<n, Iterator>::operator =(const value_type &t) const
		{
			Base::operator =(t);
			return *this;
		}
		template <unsigned n, typename Iterator> const Slice<n, Iterator> &Slice<n, Iterator>::operator =(const Vector<n, value_type> &v) const
		{
			Base::operator =(v);
			return *this;
		}

		// 2 component slice
		// constructors
		template <typename Iterator> Slice<2, Iterator>::Slice(const Iterator &iter) :
			Base(iter), x(begin()[0]), y(begin()[1]) {}
		template <typename Iterator> Slice<2, Iterator>::Slice(const Slice &other) :
			Base(other), x(begin()[0]), y(begin()[1]) {}
		template <typename Iterator> template <typename Iterator2> Slice<2, Iterator>::Slice(const Slice<2, Iterator2> &other) :
			Base(other), x(begin()[0]), y(begin()[1]) {}

		// assignment
		template <typename Iterator> const Slice<2, Iterator> &Slice<2, Iterator>::operator =(const Slice &other) const
		{
			Base::operator =(other);
			return *this;
		}
		template <typename Iterator> template <typename Iterator2> const Slice<2, Iterator> &Slice<2, Iterator>::operator =(const Slice<2, Iterator2> &other) const
		{
			Base::operator =(other);
			return *this;
		}
		template <typename Iterator> const Slice<2, Iterator> &Slice<2, Iterator>::operator =(const value_type &t) const
		{
			Base::operator =(t);
			return *this;
		}
		template <typename Iterator> const Slice<2, Iterator> &Slice<2, Iterator>::operator =(const Vector<2, value_type> &v) const
		{
			Base::operator =(v);
			return *this;
		}

		// 3 component slice
		// constructors
		template <typename Iterator> Slice<3, Iterator>::Slice(const Iterator &iter) :
			Base(iter), x(begin()[0]), y(begin()[1]), z(begin()[2]) {}
		template <typename Iterator> Slice<3, Iterator>::Slice(const Slice &other) :
			Base(other), x(begin()[0]), y(begin()[1]), z(begin()[2]) {}
		template <typename Iterator> template <typename Iterator2> Slice<3, Iterator>::Slice(const Slice<3, Iterator2> &other) :
			Base(other), x(begin()[0]), y(begin()[1]), z(begin()[2]) {}

		// assignment
		template <typename Iterator> const Slice<3, Iterator> &Slice<3, Iterator>::operator =(const Slice &other) const
		{
			Base::operator =(other);
			return *this;
		}
		template <typename Iterator> template <typename Iterator2> const Slice<3, Iterator> &Slice<3, Iterator>::operator =(const Slice<3, Iterator2> &other) const
		{
			Base::operator =(other);
			return *this;
		}
		template <typename Iterator> const Slice<3, Iterator> &Slice<3, Iterator>::operator =(const value_type &t) const
		{
			Base::operator =(t);
			return *this;
		}
		template <typename Iterator> const Slice<3, Iterator> &Slice<3, Iterator>::operator =(const Vector<3, value_type> &v) const
		{
			Base::operator =(v);
			return *this;
		}

		// 4 component slice
		// constructors
		template <typename Iterator> Slice<4, Iterator>::Slice(const Iterator &iter) :
			Base(iter), x(begin()[0]), y(begin()[1]), z(begin()[2]), w(begin()[3]) {}
		template <typename Iterator> Slice<4, Iterator>::Slice(const Slice &other) :
			Base(other), x(begin()[0]), y(begin()[1]), z(begin()[2]), w(begin()[3]) {}
		template <typename Iterator> template <typename Iterator2> Slice<4, Iterator>::Slice(const Slice<4, Iterator2> &other) :
			Base(other), x(begin()[0]), y(begin()[1]), z(begin()[2]), w(begin()[3]) {}

		// assignment
		template <typename Iterator> const Slice<4, Iterator> &Slice<4, Iterator>::operator =(const Slice &other) const
		{
			Base::operator =(other);
			return *this;
		}
		template <typename Iterator> template <typename Iterator2> const Slice<4, Iterator> &Slice<4, Iterator>::operator =(const Slice<4, Iterator2> &other) const
		{
			Base::operator =(other);
			return *this;
		}
		template <typename Iterator> const Slice<4, Iterator> &Slice<4, Iterator>::operator =(const value_type &t) const
		{
			Base::operator =(t);
			return *this;
		}
		template <typename Iterator> const Slice<4, Iterator> &Slice<4, Iterator>::operator =(const Vector<4, value_type> &v) const
		{
			Base::operator =(v);
			return *this;
		}

		// unary operators
		template <unsigned n, typename Iterator> const Slice<n, Iterator> &operator ++(const Slice<n, Iterator> &s)
		{
			std::transform(s.begin(), s.end(), s.begin(), std::bind2nd(std::plus<typename Slice<n, Iterator>::value_type>(), 1));
			return s;
		}
		template <unsigned n, typename Iterator> const Slice<n, Iterator> &operator --(const Slice<n, Iterator> &s)
		{
			std::transform(s.begin(), s.end(), s.begin(), std::bind2nd(std::minus<typename Slice<n, Iterator>::value_type>(), 1));
			return s;
		}
		template <unsigned n, typename Iterator> Vector<n, typename Slice<n, Iterator>::value_type> operator ++(const Slice<n, Iterator> &s, int)
		{
			typedef typename Slice<n, Iterator>::value_type T;
			Vector<n, T> r;
			std::transform(s.begin(), s.end(), r.begin(), std::bind2nd(std::plus<T>(), 1));
			return r;
		}
		template <unsigned n, typename Iterator> Vector<n, typename Slice<n, Iterator>::value_type> operator --(const Slice<n, Iterator> &s, int)
		{
			typedef typename Slice<n, Iterator>::value_type T;
			Vector<n, T> r;
			std::transform(s.begin(), s.end(), r.begin(), std::bind2nd(std::minus<T>(), 1));
			return r;
		}
		template <unsigned n, typename Iterator> Vector<n, typename Slice<n, Iterator>::value_type> operator +(const Slice<n, Iterator> &s)
		{
			return s;
		}
		template <unsigned n, typename Iterator> Vector<n, typename Slice<n, Iterator>::value_type> operator -(const Slice<n, Iterator> &s)
		{
			typedef typename Slice<n, Iterator>::value_type T;
			Vector<n, T> r;
			std::transform(s.begin(), s.end(), r.begin(), std::negate<T>());
			return r;
		}

		// assignment operators
		template <unsigned n, typename Iterator, typename Iterator2> const Slice<n, Iterator> &operator *=(const Slice<n, Iterator> &s1, const Slice<n, Iterator2> &s2)
		{
			std::transform(s1.begin(), s1.end(), s2.begin(), s1.begin(), std::multiplies<typename Slice<n, Iterator>::value_type>());
			return s1;
		}
		template <unsigned n, typename Iterator, typename Iterator2> const Slice<n, Iterator> &operator /=(const Slice<n, Iterator> &s1, const Slice<n, Iterator2> &s2)
		{
			std::transform(s1.begin(), s1.end(), s2.begin(), s1.begin(), std::multiplies<typename Slice<n, Iterator>::value_type>());
			return s1;
		}
		template <unsigned n, typename Iterator, typename Iterator2> const Slice<n, Iterator> &operator %=(const Slice<n, Iterator> &s1, const Slice<n, Iterator2> &s2)
		{
			std::transform(s1.begin(), s1.end(), s2.begin(), s1.begin(), util::modulus<typename Slice<n, Iterator>::value_type>());
			return s1;
		}
		template <unsigned n, typename Iterator, typename Iterator2> const Slice<n, Iterator> &operator +=(const Slice<n, Iterator> &s1, const Slice<n, Iterator2> &s2)
		{
			std::transform(s1.begin(), s1.end(), s2.begin(), s1.begin(), std::plus<typename Slice<n, Iterator>::value_type>());
			return s1;
		}
		template <unsigned n, typename Iterator, typename Iterator2> const Slice<n, Iterator> &operator -=(const Slice<n, Iterator> &s1, const Slice<n, Iterator2> &s2)
		{
			std::transform(s1.begin(), s1.end(), s2.begin(), s1.begin(), std::minus<typename Slice<n, Iterator>::value_type>());
			return s1;
		}

		// vector assignment operators
		template <unsigned n, typename Iterator, typename T> const Slice<n, Iterator> &operator *=(const Slice<n, Iterator> &s, const Vector<n, T> &v)
		{
			std::transform(s.begin(), s.end(), v.begin(), s.begin(), std::multiplies<typename Slice<n, Iterator>::value_type>());
			return s;
		}
		template <unsigned n, typename Iterator, typename T> const Slice<n, Iterator> &operator /=(const Slice<n, Iterator> &s, const Vector<n, T> &v)
		{
			std::transform(s.begin(), s.end(), v.begin(), s.begin(), std::divides<typename Slice<n, Iterator>::value_type>());
			return s;
		}
		template <unsigned n, typename Iterator, typename T> const Slice<n, Iterator> &operator %=(const Slice<n, Iterator> &s, const Vector<n, T> &v)
		{
			std::transform(s.begin(), s.end(), v.begin(), s.begin(), util::modulus<typename Slice<n, Iterator>::value_type>());
			return s;
		}
		template <unsigned n, typename Iterator, typename T> const Slice<n, Iterator> &operator +=(const Slice<n, Iterator> &s, const Vector<n, T> &v)
		{
			std::transform(s.begin(), s.end(), v.begin(), s.begin(), std::plus<typename Slice<n, Iterator>::value_type>());
			return s;
		}
		template <unsigned n, typename Iterator, typename T> const Slice<n, Iterator> &operator -=(const Slice<n, Iterator> &s, const Vector<n, T> &v)
		{
			std::transform(s.begin(), s.end(), v.begin(), s.begin(), std::minus<typename Slice<n, Iterator>::value_type>());
			return s;
		}

		// scalar assignment operators
		template <unsigned n, typename Iterator, typename T> const Slice<n, Iterator> &operator *=(const Slice<n, Iterator> &s, T t)
		{
			std::transform(s.begin(), s.end(), s.begin(), std::bind2nd(std::multiplies<typename Slice<n, Iterator>::value_type>(), t));
			return s;
		}
		template <unsigned n, typename Iterator, typename T> const Slice<n, Iterator> &operator /=(const Slice<n, Iterator> &s, T t)
		{
			std::transform(s.begin(), s.end(), s.begin(), std::bind2nd(std::divides<typename Slice<n, Iterator>::value_type>(), t));
			return s;
		}
		template <unsigned n, typename Iterator, typename T> const Slice<n, Iterator> &operator %=(const Slice<n, Iterator> &s, T t)
		{
			std::transform(s.begin(), s.end(), s.begin(), std::bind2nd(util::modulus<typename Slice<n, Iterator>::value_type>(), t));
			return s;
		}
		template <unsigned n, typename Iterator, typename T> const Slice<n, Iterator> &operator +=(const Slice<n, Iterator> &s, T t)
		{
			std::transform(s.begin(), s.end(), s.begin(), std::bind2nd(std::plus<typename Slice<n, Iterator>::value_type>(), t));
			return s;
		}
		template <unsigned n, typename Iterator, typename T> const Slice<n, Iterator> &operator -=(const Slice<n, Iterator> &s, T t)
		{
			std::transform(s.begin(), s.end(), s.begin(), std::bind2nd(std::minus<typename Slice<n, Iterator>::value_type>(), t));
			return s;
		}

		// stream insertion/extraction
		template <unsigned n, typename Iterator> std::ostream &operator <<(std::ostream &os, const Slice<n, Iterator> &s)
		{
			return util::Serialize(os, util::make_range(s), ',');
		}
		template <unsigned n, typename Iterator> std::istream &operator >>(std::istream &is, const Slice<n, Iterator> &s)
		{
			return util::Deserialize(is, util::make_range(s), util::SequenceDeserializationFlags::none, ',');
		}
	}
}
