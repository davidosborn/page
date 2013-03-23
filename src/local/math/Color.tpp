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

#include <algorithm> // copy, fill, find, {max,min}{,_element}, swap_ranges, transform
#include <functional> // bind, divides, {,not_}equal_to, {greater,less}{,_equal}, logical_{and,not,or}, minus, multiples, plus

#include "../util/algorithm/stdext.hpp" // transform_n
#include "../util/functional/operator.hpp" // modulus, unary_plus
#include "../util/io/deserialize.hpp" // Deserialize
#include "../util/io/serialize.hpp" // Serialize
#include "../util/iterator/range.hpp"
#include "float.hpp" // Wrap

namespace page
{
	namespace math
	{
		// basic color
		// constructors
		template <typename D, unsigned n, typename T> BasicColor<D, n, T>::BasicColor(T t)
		{
			std::fill(begin(), end(), t);
		}
		template <typename D, unsigned n, typename T> BasicColor<D, n, T>::BasicColor(const BasicColor &other)
		{
			std::copy(other.begin(), other.end(), begin());
		}
		template <typename D, unsigned n, typename T> template <typename D2, unsigned m, typename U> BasicColor<D, n, T>::BasicColor(const BasicColor<D2, m, U> &other)
		{
			std::fill(util::transform_n(other.begin(), std::min(n, m), begin(), [](U t){ return static_cast<T>(t); }), end(), 0);
		}
		template <typename D, unsigned n, typename T> BasicColor<D, n, T>::BasicColor(const Vector<n, T> &v)
		{
			std::copy(v.begin(), v.end(), begin());
		}

		// assignment
		template <typename D, unsigned n, typename T> BasicColor<D, n, T> &BasicColor<D, n, T>::operator =(T t)
		{
			std::fill(begin(), end(), t);
			return *this;
		}
		template <typename D, unsigned n, typename T> BasicColor<D, n, T> &BasicColor<D, n, T>::operator =(const BasicColor &other)
		{
			std::copy(other.begin(), other.end(), begin());
			return *this;
		}

		// conversion
		template <typename D, unsigned n, typename T> BasicColor<D, n, T>::operator Vector<n, T>() const
		{
			Vector<n, T> r;
			std::copy(begin(), end(), r.begin());
			return r;
		}

		// iterators
		template <typename D, unsigned n, typename T> typename BasicColor<D, n, T>::iterator BasicColor<D, n, T>::begin()
			{ return static_cast<D &>(*this)._data; }
		template <typename D, unsigned n, typename T> typename BasicColor<D, n, T>::const_iterator BasicColor<D, n, T>::begin() const
			{ return static_cast<const D &>(*this)._data; }
		template <typename D, unsigned n, typename T> typename BasicColor<D, n, T>::iterator BasicColor<D, n, T>::end()
			{ return begin() + n; }
		template <typename D, unsigned n, typename T> typename BasicColor<D, n, T>::const_iterator BasicColor<D, n, T>::end() const
			{ return begin() + n; }

		// reverse iterators
		template <typename D, unsigned n, typename T> typename BasicColor<D, n, T>::reverse_iterator BasicColor<D, n, T>::rbegin()
			{ return reverse_iterator(end()); }
		template <typename D, unsigned n, typename T> typename BasicColor<D, n, T>::const_reverse_iterator BasicColor<D, n, T>::rbegin() const
			{ return const_reverse_iterator(end()); }
		template <typename D, unsigned n, typename T> typename BasicColor<D, n, T>::reverse_iterator BasicColor<D, n, T>::rend()
			{ return reverse_iterator(begin()); }
		template <typename D, unsigned n, typename T> typename BasicColor<D, n, T>::const_reverse_iterator BasicColor<D, n, T>::rend() const
			{ return const_reverse_iterator(begin()); }

		// element access
		template <typename D, unsigned n, typename T> typename BasicColor<D, n, T>::reference BasicColor<D, n, T>::operator [](unsigned i)
			{ return begin()[i]; }
		template <typename D, unsigned n, typename T> typename BasicColor<D, n, T>::const_reference BasicColor<D, n, T>::operator [](unsigned i) const
			{ return begin()[i]; }

		// color
		// constructors
		template <unsigned n, typename T> Color<n, T>::Color(T t) :
			Base(t) {}
		template <unsigned n, typename T> template <unsigned m, typename U> Color<n, T>::Color(const Color<m, U> &other) :
			Base(other) {}
		template <unsigned n, typename T> Color<n, T>::Color(const Vector<n, T> &v) :
			Base(v) {}

		// assignment
		template <unsigned n, typename T> Color<n, T> &Color<n, T>::operator =(T t)
		{
			Base::operator =(t);
			return *this;
		}

		// conversion
		template <unsigned n, typename T> template <typename U> Color<n, T>::operator Color<n, U>() const
		{
			Color<n, U> r;
			std::copy(begin(), end(), r.begin());
			return r;
		}

		// HSV color
		// constructors
		template <typename T> HsvColor<T>::HsvColor(T t) :
			Base(t) {}
		template <typename T> HsvColor<T>::HsvColor(T h, T s, T v) :
			h(h), s(s), v(v) {}
		template <typename T> template <typename U> HsvColor<T>::HsvColor(const HsvColor<U> &other) :
			Base(other) {}
		template <typename T> HsvColor<T>::HsvColor(const RgbColor<T> &rgb)
		{
			// RGB to HSV conversion algorithm from
			// http://www.cs.rit.edu/~ncs/color/t_convert.html
			T
				min = Min(rgb),
				max = Max(rgb),
				delta = max - min;
			h = delta ? (
				max == rgb.r ?     (rgb.g - rgb.b) / (delta * 6) :
				max == rgb.g ? (2 + rgb.b - rgb.r) / (delta * 6) :
				               (4 + rgb.r - rgb.g) / (delta * 6)) : 0;
			if (h < 0) h += 1;
			s = max ? delta / max : 0;
			v = max;
		}
		template <typename T> HsvColor<T>::HsvColor(const RgbaColor<T> &rgba) :
			Base(HsvColor(RgbColor<T>(rgba))) {}
		template <typename T> HsvColor<T>::HsvColor(const Vector<3, T> &v) :
			Base(v) {}

		// assignment
		template <typename T> HsvColor<T> &HsvColor<T>::operator =(T t)
		{
			Base::operator =(t);
			return *this;
		}

		// conversion
		template <typename T> template <typename U> HsvColor<T>::operator HsvColor<U>() const
		{
			HsvColor<U> r;
			std::copy(begin(), end(), r.begin());
			return r;
		}

		// RGB color
		// constructors
		template <typename T> RgbColor<T>::RgbColor(T t) :
			Base(t) {}
		template <typename T> RgbColor<T>::RgbColor(T r, T g, T b) :
			r(r), g(g), b(b) {}
		template <typename T> template <typename U> RgbColor<T>::RgbColor(const RgbColor<U> &other) :
			Base(other) {}
		template <typename T> RgbColor<T>::RgbColor(const HsvColor<T> &hsv)
		{
			// HSV to RGB conversion algorithm from
			// http://www.cs.rit.edu/~ncs/color/t_convert.html
			unsigned i = hsv.h * 6;
			T
				f = hsv.h * 6 - i,
				p = hsv.v * (1 - hsv.s),
				q = hsv.v * (1 - hsv.s * f),
				t = hsv.v * (1 - hsv.s * (1 - f));
			switch (i)
			{
				case 0:  r = hsv.v; g = t;     b = p;     break;
				case 1:  r = q;     g = hsv.v; b = p;     break;
				case 2:  r = p;     g = hsv.v; b = t;     break;
				case 3:  r = p;     g = q;     b = hsv.v; break;
				case 4:  r = t;     g = p;     b = hsv.v; break;
				default: r = hsv.v; g = p;     b = q;
			}
		}
		template <typename T> RgbColor<T>::RgbColor(const RgbaColor<T> &rgba) :
			Base(rgba) {}
		template <typename T> RgbColor<T>::RgbColor(const Vector<3, T> &v) :
			Base(v) {}
		template <typename T> RgbColor<T>::RgbColor(const YcbcrColor<T> &ycbcr) :
			Base(YcbcrToRgbColorMatrix<T>() * Vector<3, T>(ycbcr)) {}

		// assignment
		template <typename T> RgbColor<T> &RgbColor<T>::operator =(T t)
		{
			Base::operator =(t);
			return *this;
		}

		// conversion
		template <typename T> template <typename U> RgbColor<T>::operator RgbColor<U>() const
		{
			RgbColor<U> r;
			std::copy(begin(), end(), r.begin());
			return r;
		}

		// RGBA color
		// constructors
		template <typename T> RgbaColor<T>::RgbaColor(T t, T a) :
			Base(t), a(a) {}
		template <typename T> RgbaColor<T>::RgbaColor(T r, T g, T b, T a) :
			r(r), g(g), b(b), a(a) {}
		template <typename T> template <typename U> RgbaColor<T>::RgbaColor(const RgbaColor<U> &other) :
			Base(other) {}
		template <typename T> RgbaColor<T>::RgbaColor(const HsvColor<T> &hsv, T a) :
			Base(RgbaColor(RgbColor<T>(hsv), a)) {}
		template <typename T> RgbaColor<T>::RgbaColor(const RgbColor<T> &rgb, T a) :
			Base(rgb), a(a) {}
		template <typename T> RgbaColor<T>::RgbaColor(const Vector<4, T> &v) :
			Base(v) {}
		template <typename T> RgbaColor<T>::RgbaColor(const YcbcrColor<T> &ycbcr, T a) :
			Base(RgbaColor(RgbColor<T>(ycbcr), a)) {}

		// assignment
		template <typename T> RgbaColor<T> &RgbaColor<T>::operator =(T t)
		{
			Base::operator =(t);
			return *this;
		}

		// conversion
		template <typename T> template <typename U> RgbaColor<T>::operator RgbaColor<U>() const
		{
			RgbaColor<U> r;
			std::copy(begin(), end(), r.begin());
			return r;
		}

		// Y'CbCr color
		// constructors
		template <typename T> YcbcrColor<T>::YcbcrColor(T t) :
			Base(t) {}
		template <typename T> YcbcrColor<T>::YcbcrColor(T y, T cb, T cr) :
			y(y), cb(cb), cr(cr) {}
		template <typename T> template <typename U> YcbcrColor<T>::YcbcrColor(const YcbcrColor<U> &other) :
			Base(other) {}
		template <typename T> YcbcrColor<T>::YcbcrColor(const RgbColor<T> &rgb) :
			Base(RgbToYcbcrColorMatrix<T>() * Vector<3, T>(rgb)) {}
		template <typename T> YcbcrColor<T>::YcbcrColor(const RgbaColor<T> &rgba) :
			Base(YcbcrColor(RgbColor<T>(rgba))) {}
		template <typename T> YcbcrColor<T>::YcbcrColor(const Vector<3, T> &v) :
			Base(v) {}

		// assignment
		template <typename T> YcbcrColor<T> &YcbcrColor<T>::operator =(T t)
		{
			Base::operator =(t);
			return *this;
		}

		// conversion
		template <typename T> template <typename U> YcbcrColor<T>::operator YcbcrColor<U>() const
		{
			YcbcrColor<U> r;
			std::copy(begin(), end(), r.begin());
			return r;
		}

		// RGB initialization
		template <typename T> RgbColor<T> BlackRgbColor()
		{
			return RgbColor<T>();
		}
		template <typename T> RgbColor<T> WhiteRgbColor()
		{
			return RgbColor<T>(1);
		}
		template <typename T> RgbColor<T> LuminanceCoefficientRgbColor()
		{
			// http://www.faqs.org/faqs/graphics/colorspace-faq/
			return RgbColor<T>(.212671, .715160, .072169);
		}

		// RGBA initialization
		template <typename T> RgbaColor<T> ClearRgbaColor()
		{
			return RgbaColor<T>(0, 0);
		}
		template <typename T> RgbaColor<T> BlackRgbaColor()
		{
			return RgbaColor<T>();
		}
		template <typename T> RgbaColor<T> WhiteRgbaColor()
		{
			return RgbaColor<T>(1);
		}
		template <typename T> RgbaColor<T> LuminanceCoefficientRgbaColor()
		{
			return RgbaColor<T>(LuminanceCoefficientRgbColor<T>());
		}

		// Y'CbCr initialization
		template <typename T> YcbcrColor<T> YcbcrBiasColor()
		{
			// http://www.mir.com/DMG/ycbcr.html
			return YcbcrColor<T>(16, 128, 128) / 256;
		}
		template <typename T> YcbcrColor<T> YcbcrScaleColor()
		{
			// http://www.mir.com/DMG/ycbcr.html
			return YcbcrColor<T>(219, 224, 224) / 256;
		}

		// matrix initialization
		template <typename T> Matrix<3, 3, T> RgbToYcbcrColorMatrix()
		{
			// http://www.mir.com/DMG/ycbcr.html
			return Matrix<3, 3, T>(
				 .299,     .587,     .114,
				-.168736, -.331264,  .5,
				 .5,      -.418688, -.081312);
		}
		template <typename T> Matrix<3, 3, T> YcbcrToRgbColorMatrix()
		{
			// http://www.mir.com/DMG/ycbcr.html
			return Matrix<3, 3, T>(
				1,  0,         1.402,
				1, -0.344136, -0.714136,
				1,  1.772,     0);
		}

		// operators
#define DEFINE_BINARY_ARITHMETIC_OPERATOR(TYPE, OP, FUNC) \
	template <typename T, typename U> TYPE<typename ArithmeticConversion<T, U>::Result> operator OP(const TYPE<T> &c1, const TYPE<U> &c2) \
	{ \
		typedef typename ArithmeticConversion<T, U>::Result R; \
		TYPE<R> r; \
		std::transform(c1.begin(), c1.end(), c2.begin(), r.begin(), FUNC<R>()); \
		return r; \
	} \
	template <typename T, typename U> TYPE<typename ArithmeticConversion<T, U>::Result> operator OP(const TYPE<T> &c, U t) \
	{ \
		typedef typename ArithmeticConversion<T, U>::Result R; \
		TYPE<R> r; \
		std::transform(c.begin(), c.end(), r.begin(), std::bind(FUNC<R>(), std::placeholders::_1, t)); \
		return r; \
	} \
	template <typename T, typename U> TYPE<typename ArithmeticConversion<T, U>::Result> operator OP(T t, const TYPE<U> &c) \
	{ \
		typedef typename ArithmeticConversion<T, U>::Result R; \
		TYPE<R> r; \
		std::transform(c.begin(), c.end(), r.begin(), std::bind(FUNC<R>(), t, std::placeholders::_1)); \
		return r; \
	}

#define DEFINE_UNARY_ARITHMETIC_OPERATOR(TYPE, OP, FUNC) \
	template <typename T> TYPE<T> operator OP(const TYPE<T> &c) \
	{ \
		TYPE<bool> r; \
		std::transform(c.begin(), c.end(), r.begin(), FUNC<T>()); \
		return r; \
	}

#define DEFINE_BINARY_LOGICAL_OPERATOR(TYPE, OP, FUNC) \
	template <typename T, typename U> TYPE<bool> operator OP(const TYPE<T> &c1, const TYPE<U> &c2) \
	{ \
		TYPE<bool> r; \
		std::transform(c1.begin(), c1.end(), c2.begin(), r.begin(), FUNC<typename ArithmeticConversion<T, U>::Result>()); \
		return r; \
	} \
	template <typename T, typename U> TYPE<bool> operator OP(const TYPE<T> &c, U t) \
	{ \
		TYPE<bool> r; \
		std::transform(c.begin(), c.end(), r.begin(), std::bind(FUNC<typename ArithmeticConversion<T, U>::Result>(), std::placeholders::_1, t)); \
		return r; \
	} \
	template <typename T, typename U> TYPE<bool> operator OP(T t, const TYPE<U> &c) \
	{ \
		TYPE<bool> r; \
		std::transform(c.begin(), c.end(), r.begin(), std::bind(FUNC<typename ArithmeticConversion<T, U>::Result>(), t, std::placeholders::_1)); \
		return r; \
	}

#define DEFINE_UNARY_LOGICAL_OPERATOR(TYPE, OP, FUNC) \
	template <typename T> TYPE<bool> operator OP(const TYPE<T> &c) \
	{ \
		TYPE<bool> r; \
		std::transform(c.begin(), c.end(), r.begin(), FUNC<T>()); \
		return r; \
	}

#define DEFINE_ASSIGNMENT_OPERATOR(TYPE, OP, FUNC) \
	template <typename T, typename U> TYPE<T> &operator OP##=(TYPE<T> &c1, const TYPE<U> &c2) \
	{ \
		std::transform(c1.begin(), c1.end(), c2.begin(), c1.begin(), FUNC<T>()); \
		return c1; \
	} \
	template <typename T, typename U> TYPE<T> &operator OP##=(TYPE<T> &c, U t) \
	{ \
		std::transform(c.begin(), c.end(), c.begin(), std::bind(FUNC<T>(), std::placeholders::_1, t)); \
		return c; \
	}

#define DEFINE_OPERATORS(TYPE) \
	DEFINE_BINARY_ARITHMETIC_OPERATOR(TYPE, *,  std::multiplies) \
	DEFINE_BINARY_ARITHMETIC_OPERATOR(TYPE, /,  std::divides) \
	DEFINE_BINARY_ARITHMETIC_OPERATOR(TYPE, %,  util::modulus) \
	DEFINE_BINARY_ARITHMETIC_OPERATOR(TYPE, +,  std::plus) \
	DEFINE_BINARY_ARITHMETIC_OPERATOR(TYPE, -,  std::minus) \
	DEFINE_UNARY_ARITHMETIC_OPERATOR (TYPE, +,  util::unary_plus) \
	DEFINE_UNARY_ARITHMETIC_OPERATOR (TYPE, -,  std::negate) \
	DEFINE_BINARY_LOGICAL_OPERATOR   (TYPE, <,  std::less) \
	DEFINE_BINARY_LOGICAL_OPERATOR   (TYPE, >,  std::greater) \
	DEFINE_BINARY_LOGICAL_OPERATOR   (TYPE, <=, std::less_equal) \
	DEFINE_BINARY_LOGICAL_OPERATOR   (TYPE, >=, std::greater_equal) \
	DEFINE_BINARY_LOGICAL_OPERATOR   (TYPE, ==, std::equal_to) \
	DEFINE_BINARY_LOGICAL_OPERATOR   (TYPE, !=, std::not_equal_to) \
	DEFINE_BINARY_LOGICAL_OPERATOR   (TYPE, &&, std::logical_and) \
	DEFINE_BINARY_LOGICAL_OPERATOR   (TYPE, ||, std::logical_or) \
	DEFINE_UNARY_LOGICAL_OPERATOR    (TYPE, !,  std::logical_not) \
	DEFINE_ASSIGNMENT_OPERATOR       (TYPE, *,  std::multiplies) \
	DEFINE_ASSIGNMENT_OPERATOR       (TYPE, /,  std::divides) \
	DEFINE_ASSIGNMENT_OPERATOR       (TYPE, %,  util::modulus) \
	DEFINE_ASSIGNMENT_OPERATOR       (TYPE, +,  std::plus) \
	DEFINE_ASSIGNMENT_OPERATOR       (TYPE, -,  std::minus)

		DEFINE_OPERATORS(HsvColor)
		DEFINE_OPERATORS(RgbColor)
		DEFINE_OPERATORS(RgbaColor)
		DEFINE_OPERATORS(YcbcrColor)

		// boolean combiners
		template <typename D, unsigned n, typename T> bool All(const BasicColor<D, n, T> &c)
		{
			Vector<n, bool> b(c.begin(), c.end());
			return std::find(b.begin(), b.end(), false) == b.end();
		}
		template <typename D, unsigned n, typename T> bool Any(const BasicColor<D, n, T> &c)
		{
			Vector<n, bool> b(c.begin(), c.end());
			return std::find(b.begin(), b.end(), true) != b.end();
		}

		// min/max functions
		template <typename D, unsigned n, typename T> T Min(const BasicColor<D, n, T> &c)
		{
			return *std::min_element(c.begin(), c.end());
		}
		template <typename D, unsigned n, typename T> T Max(const BasicColor<D, n, T> &c)
		{
			return *std::max_element(c.begin(), c.end());
		}
		template <typename D1, typename D2, unsigned n, typename T, typename U> Color<n, typename ArithmeticConversion<T, U>::Result> Min(const BasicColor<D1, n, T> &c1, const BasicColor<D2, n, U> &c2)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Color<n, R> r;
			std::transform(c1.begin(), c1.end(), c2.begin(), r.begin(), [](T c1, U c2){ return std::min<R>(c1, c2); });
			return r;
		}
		template <typename D, unsigned n, typename T, typename U> Color<n, typename ArithmeticConversion<T, U>::Result> Min(const BasicColor<D, n, T> &c, U t)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Color<n, R> r;
			std::transform(c.begin(), c.end(), r.begin(), [&t](T c){ return std::min<R>(c, t); });
			return r;
		}
		template <typename D, unsigned n, typename T, typename U> Color<n, typename ArithmeticConversion<T, U>::Result> Min(T t, const BasicColor<D, n, U> &c)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Color<n, R> r;
			std::transform(c.begin(), c.end(), r.begin(), [&t](U c){ return std::min<R>(t, c); });
			return r;
		}
		template <typename D1, typename D2, unsigned n, typename T, typename U> Color<n, typename ArithmeticConversion<T, U>::Result> Max(const BasicColor<D1, n, T> &c1, const BasicColor<D2, n, U> &c2)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Color<n, R> r;
			std::transform(c1.begin(), c1.end(), c2.begin(), r.begin(), [](T c1, U c2){ return std::max<R>(c1, c2); });
			return r;
		}
		template <typename D, unsigned n, typename T, typename U> Color<n, typename ArithmeticConversion<T, U>::Result> Max(const BasicColor<D, n, T> &c, U t)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Color<n, R> r;
			std::transform(c.begin(), c.end(), r.begin(), [&t](T c){ return std::max<R>(c, t); });
			return r;
		}
		template <typename D, unsigned n, typename T, typename U> Color<n, typename ArithmeticConversion<T, U>::Result> Max(T t, const BasicColor<D, n, U> &c)
		{
			typedef typename ArithmeticConversion<T, U>::Result R;
			Color<n, R> r;
			std::transform(c.begin(), c.end(), r.begin(), [&t](U c){ return std::max<R>(t, c); });
			return r;
		}

		// color transformation
		template <typename T> HsvColor<T> Complement(const HsvColor<T> &c)
		{
			return HsvColor<T>(Wrap(c.h + T(.5), 0, 1), c.s, c.v);
		}
		template <typename T> RgbColor<T> MultiplyAlpha(const RgbaColor<T> &c)
		{
			return RgbColor<T>(c) * c.a;
		}
		template <typename T> YcbcrColor<T> SubRange(const YcbcrColor<T> &c)
		{
			return c * YcbcrColor<T>(.85546875, .875, .875) + .0625;
		}
		template <typename T> YcbcrColor<T> ResetRange(const YcbcrColor<T> &c)
		{
			return (c - .0625) / YcbcrColor<T>(.85546875, .875, .875);
		}

		// stream insertion/extraction
		template <typename D, unsigned n, typename T> std::ostream &operator <<(std::ostream &os, const BasicColor<D, n, T> &c)
		{
			return util::Serialize(os, util::make_range(c), ',');
		}
		template <typename D, unsigned n, typename T> std::istream &operator >>(std::istream &is, BasicColor<D, n, T> &c)
		{
			return util::Deserialize(is, util::make_range(c), util::SequenceDeserializationFlags::none, ',');
		}

		// specialized algorithms
		template <typename D, unsigned n, typename T> void swap(BasicColor<D, n, T> &a, BasicColor<D, n, T> &b)
		{
			std::swap_ranges(a.begin(), a.end(), b.begin());
		}
	}
}
