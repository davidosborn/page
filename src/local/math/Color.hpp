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

#ifndef    page_local_math_Color_hpp
#   define page_local_math_Color_hpp

#	include <cstddef> // ptrdiff_t, size_t
#	include <iosfwd> // [io]stream
#	include <iterator> // reverse_iterator
#	include "ArithmeticConversion.hpp"
#	include "fwd.hpp" // {,Hsv,Rgb,Rgba,Ycbcr}Color, defaultType
#	include "Matrix.hpp"
#	include "Vector.hpp"

namespace page
{
	namespace math
	{
		/**
		 * @class BasicColor
		 *
		 * The base class of @c Color, providing a standard implementation
		 * which all specializations share.
		 */
		template <typename Derived, unsigned n, typename T> struct BasicColor
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
			BasicColor(T = 0);
			BasicColor(const BasicColor &);
			template <typename D2, unsigned m, typename U> explicit BasicColor(const BasicColor<D2, m, U> &);
			explicit BasicColor(const Vector<n, T> &);

			// assignment
			BasicColor &operator =(T);
			BasicColor &operator =(const BasicColor &);

			public:
			// conversion
			explicit operator Vector<n, T>() const;

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
			reference operator [](unsigned);
			const_reference operator [](unsigned) const;
		};

		/**
		 * @name Color
		 *
		 * Classes for mathematically representing color in various formats.
		 *
		 * @{
		 */
		template <unsigned n, typename T> struct Color : BasicColor<Color<n, T>, n, T>
		{
			typedef BasicColor<Color<n, T>, n, T> Base;

			// constructors
			Color(T = 0);
			template <unsigned m, typename U> explicit Color(const Color<m, U> &);
			explicit Color(const Vector<n, T> &);

			// assignment
			Color &operator =(T);

			// conversion
			template <typename U> operator Color<n, U>() const;

			using Base::begin;
			using Base::end;

			T _data[n]; // HACK: conceptually private
		};
		template <typename T> struct HsvColor : BasicColor<HsvColor<T>, 3, T>
		{
			typedef BasicColor<HsvColor<T>, 3, T> Base;

			// constructors
			HsvColor(T = 0);
			HsvColor(T, T, T);
			template <typename U> explicit HsvColor(const HsvColor<U> &);
			explicit HsvColor(const RgbColor<T> &);
			explicit HsvColor(const RgbaColor<T> &);
			explicit HsvColor(const Vector<3, T> &);

			// assignment
			HsvColor &operator =(T);

			// conversion
			template <typename U> operator HsvColor<U>() const;

			using Base::begin;
			using Base::end;

			union
			{
				__extension__ struct { T h, s, v; };
				__extension__ struct { T hue, saturation, value; };
				T _data[3]; // HACK: conceptually private
			};
		};
		template <typename T> struct RgbColor : BasicColor<RgbColor<T>, 3, T>
		{
			typedef BasicColor<RgbColor<T>, 3, T> Base;

			// constructors
			RgbColor(T = 0);
			RgbColor(T, T, T);
			template <typename U> explicit RgbColor(const RgbColor<U> &);
			explicit RgbColor(const HsvColor<T> &);
			explicit RgbColor(const RgbaColor<T> &);
			explicit RgbColor(const Vector<3, T> &);
			explicit RgbColor(const YcbcrColor<T> &);

			// assignment
			RgbColor &operator =(T);

			// conversion
			template <typename U> operator RgbColor<U>() const;

			using Base::begin;
			using Base::end;

			union
			{
				__extension__ struct { T r, g, b; };
				__extension__ struct { T red, green, blue; };
				T _data[3]; // HACK: conceptually private
			};
		};
		template <typename T> struct RgbaColor : BasicColor<RgbaColor<T>, 4, T>
		{
			typedef BasicColor<RgbaColor<T>, 4, T> Base;

			// constructors
			RgbaColor(T = 0, T = 1);
			RgbaColor(T, T, T, T = 1);
			template <typename U> explicit RgbaColor(const RgbaColor<U> &);
			explicit RgbaColor(const HsvColor<T> &, T = 1);
			explicit RgbaColor(const RgbColor<T> &, T = 1);
			explicit RgbaColor(const Vector<4, T> &);
			explicit RgbaColor(const YcbcrColor<T> &, T = 1);

			// assignment
			RgbaColor &operator =(T);

			// conversion
			template <typename U> operator RgbaColor<U>() const;

			using Base::begin;
			using Base::end;

			union
			{
				__extension__ struct { T r, g, b, a; };
				__extension__ struct { T red, green, blue, alpha; };
				T _data[4]; // HACK: conceptually private
			};
		};
		template <typename T> struct YcbcrColor : BasicColor<YcbcrColor<T>, 3, T>
		{
			typedef BasicColor<YcbcrColor<T>, 3, T> Base;

			// constructors
			YcbcrColor(T = 0);
			YcbcrColor(T, T, T);
			template <typename U> explicit YcbcrColor(const YcbcrColor<U> &);
			explicit YcbcrColor(const RgbColor<T> &);
			explicit YcbcrColor(const RgbaColor<T> &);
			explicit YcbcrColor(const Vector<3, T> &);

			// assignment
			YcbcrColor &operator =(T);

			// conversion
			template <typename U> operator YcbcrColor<U>() const;

			using Base::begin;
			using Base::end;

			union
			{
				__extension__ struct { T y, cb, cr; };
				__extension__ struct { T luma, blue, red; };
				T _data[3]; // HACK: conceptually private
			};
		};
		/**
		 * @}
		 */

		// RGB initialization
		template <typename T = defaultType> RgbColor<T> BlackRgbColor();
		template <typename T = defaultType> RgbColor<T> WhiteRgbColor();
		template <typename T = defaultType> RgbColor<T> LuminanceCoefficientRgbColor();

		// RGBA initialization
		template <typename T = defaultType> RgbaColor<T> ClearRgbaColor();
		template <typename T = defaultType> RgbaColor<T> BlackRgbaColor();
		template <typename T = defaultType> RgbaColor<T> WhiteRgbaColor();
		template <typename T = defaultType> RgbaColor<T> LuminanceCoefficientRgbaColor();

		// Y'CbCr initialization
		template <typename T = defaultType> YcbcrColor<T> YcbcrBiasColor();
		template <typename T = defaultType> YcbcrColor<T> YcbcrScaleColor();

		// matrix initialization
		template <typename T = defaultType> Matrix<3, 3, T> RgbToYcbcrColorMatrix();
		template <typename T = defaultType> Matrix<3, 3, T> YcbcrToRgbColorMatrix();

		// operators
#	define DECLARE_BINARY_ARITHMETIC_OPERATOR(TYPE, OP) \
		template <typename T, typename U> \
			TYPE<typename ArithmeticConversion<T, U>::Result> \
			operator OP(const TYPE<T> &, const TYPE<U> &); \
		template <typename T, typename U> \
			TYPE<typename ArithmeticConversion<T, U>::Result> \
			operator OP(const TYPE<T> &, U); \
		template <typename T, typename U> \
			TYPE<typename ArithmeticConversion<T, U>::Result> \
			operator OP(T, const TYPE<U> &);

#	define DECLARE_UNARY_ARITHMETIC_OPERATOR(TYPE, OP) \
		template <typename T> TYPE<T> operator OP(const TYPE<T> &);

#	define DECLARE_BINARY_LOGICAL_OPERATOR(TYPE, OP) \
		template <typename T, typename U> \
			TYPE<bool> operator OP(const TYPE<T> &, const TYPE<U> &); \
		template <typename T, typename U> \
			TYPE<bool> operator OP(const TYPE<T> &, U); \
		template <typename T, typename U> \
			TYPE<bool> operator OP(T, const TYPE<U> &); \

#	define DECLARE_UNARY_LOGICAL_OPERATOR(TYPE, OP) \
		template <typename T> TYPE<bool> operator OP(const TYPE<T> &);

#	define DECLARE_ASSIGNMENT_OPERATOR(TYPE, OP) \
		template <typename T, typename U> \
			TYPE<T> &operator OP##=(TYPE<T> &, const TYPE<U> &); \
		template <typename T, typename U> \
			TYPE<T> &operator OP##=(TYPE<T> &, U);

#	define DECLARE_OPERATORS(TYPE) \
		DECLARE_BINARY_ARITHMETIC_OPERATOR(TYPE, *) \
		DECLARE_BINARY_ARITHMETIC_OPERATOR(TYPE, /) \
		DECLARE_BINARY_ARITHMETIC_OPERATOR(TYPE, %) \
		DECLARE_BINARY_ARITHMETIC_OPERATOR(TYPE, +) \
		DECLARE_BINARY_ARITHMETIC_OPERATOR(TYPE, -) \
		DECLARE_UNARY_ARITHMETIC_OPERATOR (TYPE, +) \
		DECLARE_UNARY_ARITHMETIC_OPERATOR (TYPE, -) \
		DECLARE_BINARY_LOGICAL_OPERATOR   (TYPE, <) \
		DECLARE_BINARY_LOGICAL_OPERATOR   (TYPE, >) \
		DECLARE_BINARY_LOGICAL_OPERATOR   (TYPE, <=) \
		DECLARE_BINARY_LOGICAL_OPERATOR   (TYPE, >=) \
		DECLARE_BINARY_LOGICAL_OPERATOR   (TYPE, ==) \
		DECLARE_BINARY_LOGICAL_OPERATOR   (TYPE, !=) \
		DECLARE_BINARY_LOGICAL_OPERATOR   (TYPE, &&) \
		DECLARE_BINARY_LOGICAL_OPERATOR   (TYPE, ||) \
		DECLARE_UNARY_LOGICAL_OPERATOR    (TYPE, !) \
		DECLARE_ASSIGNMENT_OPERATOR       (TYPE, *) \
		DECLARE_ASSIGNMENT_OPERATOR       (TYPE, /) \
		DECLARE_ASSIGNMENT_OPERATOR       (TYPE, %) \
		DECLARE_ASSIGNMENT_OPERATOR       (TYPE, +) \
		DECLARE_ASSIGNMENT_OPERATOR       (TYPE, -)

		DECLARE_OPERATORS(HsvColor)
		DECLARE_OPERATORS(RgbColor)
		DECLARE_OPERATORS(RgbaColor)
		DECLARE_OPERATORS(YcbcrColor)

		// boolean combiners
		template <typename D, unsigned n, typename T> bool All(const BasicColor<D, n, T> &);
		template <typename D, unsigned n, typename T> bool Any(const BasicColor<D, n, T> &);

		// min/max functions
		template <typename D, unsigned n, typename T> T Min(const BasicColor<D, n, T> &);
		template <typename D, unsigned n, typename T> T Max(const BasicColor<D, n, T> &);
		template <typename D1, typename D2, unsigned n, typename T, typename U> Color<n, typename ArithmeticConversion<T, U>::Result> Min(const BasicColor<D1, n, T> &, const BasicColor<D2, n, U> &);
		template <typename D, unsigned n, typename T, typename U> Color<n, typename ArithmeticConversion<T, U>::Result> Min(const BasicColor<D, n, T> &, U);
		template <typename D, unsigned n, typename T, typename U> Color<n, typename ArithmeticConversion<T, U>::Result> Min(T, const BasicColor<D, n, U> &);
		template <typename D1, typename D2, unsigned n, typename T, typename U> Color<n, typename ArithmeticConversion<T, U>::Result> Max(const BasicColor<D1, n, T> &, const BasicColor<D2, n, U> &);
		template <typename D, unsigned n, typename T, typename U> Color<n, typename ArithmeticConversion<T, U>::Result> Max(const BasicColor<D, n, T> &, U);
		template <typename D, unsigned n, typename T, typename U> Color<n, typename ArithmeticConversion<T, U>::Result> Max(T, const BasicColor<D, n, U> &);

		// color transformations
		template <typename T> HsvColor<T> Complement(const HsvColor<T> &);
		template <typename T> RgbColor<T> MultiplyAlpha(const RgbaColor<T> &);
		template <typename T> YcbcrColor<T> SubRange(const YcbcrColor<T> &);
		template <typename T> YcbcrColor<T> ResetRange(const YcbcrColor<T> &);

		// stream insertion/extraction
		template <typename D, unsigned n, typename T> std::ostream &operator <<(std::ostream &, const BasicColor<D, n, T> &);
		template <typename D, unsigned n, typename T> std::istream &operator >>(std::istream &, BasicColor<D, n, T> &);

		// specialized algorithms
		template <typename D, unsigned n, typename T> void swap(BasicColor<D, n, T> &, BasicColor<D, n, T> &);
	}
}

#	include "Color.tpp"
#endif
