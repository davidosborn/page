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

#ifndef    page_math_float_hpp
#   define page_math_float_hpp

#	include <functional> // binary_function
#	include <limits> // numeric_limits

#	include "ArithmeticConversion.hpp"
#	include "fwd.hpp" // DefaultType

namespace page { namespace math
{
	/*----------------+
	| type conversion |
	+----------------*/

	namespace detail
	{
		template <typename T, bool = std::numeric_limits<T>::is_integer>
			struct RealImpl
		{
			typedef T type;
		};
		template <typename T>
			struct RealImpl<T, true>
		{
			typedef DefaultType type;
		};
	}

	/**
	 * Determines the appropriate real type for a numeric type.  An integer type
	 * yields the default real type, which non-integer types yield the original
	 * type.
	 */
	template <typename T>
		struct Real : detail::RealImpl<T> {};

	/*------------------+
	| special constants |
	+------------------*/

	template <typename T = float> T Inf();
	template <typename T = float> T Nan();
	template <typename T = float> T Pi();
	template <typename T = float> T E();
	template <typename T = float> T Phi(); // golden ratio

	/*-----------------------------+
	| special constant comparisons |
	+-----------------------------*/

	template <typename T> bool IsInf(T);
	template <typename T> bool IsNan(T);

	/*-----------------------------+
	| tolerant equality comparison |
	+-----------------------------*/

	template <typename T, typename U> bool Near(T, U);

	/*-------------+
	| quantization |
	+-------------*/

	template <typename T> T Round(T);
	template <typename T, typename U>
		typename ArithmeticConversion<T, U>::Result Round(T, U precision);

	/*---------+
	| wrapping |
	+---------*/

	template <typename T, typename U>
		typename ArithmeticConversion<T, U>::Result Wrap(T, U min, U max);

	/*------------------+
	| angle conversions |
	+------------------*/

	template <typename T> T DegToRad(T);
	template <typename T> T DegToRev(T);
	template <typename T> T RadToDeg(T);
	template <typename T> T RadToRev(T);
	template <typename T> T RevToDeg(T);
	template <typename T> T RevToRad(T);

	/*-------------------+
	| angle calculations |
	+-------------------*/

	template <typename T> T AngleGivenSides(T, T, T);
	template <typename T> T SideGivenSidesAngle(T, T, T);
	template <typename T> T FirstAngleGivenSidesAngle(T, T, T);
	template <typename T> T SecondAngleGivenSidesAngle(T, T, T);

	/*-------------------------+
	| audio-volume conversions |
	+-------------------------*/

	template <typename T> T DecibelToLinear(T);
	template <typename T> T LinearToDecibel(T);
}}

#	include "float.tpp"
#endif
