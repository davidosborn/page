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

#ifndef    page_math_interp_hpp
#   define page_math_interp_hpp

#	include <array>

#	include "fwd.hpp" // Quat

namespace page { namespace math
{
	/*-----------------------+
	| standard interpolation |
	+-----------------------*/

	/**
	 * Linear interpolation.
	 */
	template <typename T, typename U>
		T Lerp(T, T, U);

	/**
	 * Linear interpolation, normalized for quaternions.
	 */
	template <typename T, typename U>
		Quat<T> Lerp(const Quat<T> &, const Quat<T> &, U);

	/**
	 * Linear interpolation, normalized for quaternions, and without the
	 * directional check.
	 */
	template <typename T, typename U>
		Quat<T> LerpUnchecked(const Quat<T> &, const Quat<T> &, U);

	/**
	 * Spherical linear interpolation.
	 *
	 * @sa Van Verth, Essential Mathematics for Games, 2004
	 */
	template <typename T, typename U>
		Quat<T> Slerp(const Quat<T> &, const Quat<T> &, U);

	/**
	 * Jonathan Blow's fast implementation of slerp.
	 *
	 * @sa Van Verth, Essential Mathematics for Games, 2004
	 * @sa Jonathan Blow, Hacking Quaternions, Game Developer, March 2002
	 */
	template <typename T, typename U>
		Quat<T> FastSlerp(const Quat<T> &, const Quat<T> &, U);

	/**
	 * Spherical quadratic interpolation.
	 */
	template <typename T, typename U>
		Quat<T> Squad(const Quat<T> &, const Quat<T> &, const Quat<T> &, const Quat<T> &, U);

	/**
	 * Bilinear interpolation.
	 */
	template <typename T, typename U>
		T Bilerp(T, T, T, U, U);

	/**
	 * Bilinear interpolation, normalized for quaternions.
	 */
	template <typename T, typename U>
		Quat<T> Bilerp(const Quat<T> &, const Quat<T> &, const Quat<T> &, U, U);

	/*---------------------+
	| linear interpolation |
	+---------------------*/

	/**
	 * @defgroup
	 * A standard entry-point for linear interpolation, which delegates to the
	 * most appropriate implementation for the type.
	 * @{
	 */
	template <typename T, typename U>
		T LinearInterp(T, T, U);
	template <typename T, typename U>
		Quat<T> LinearInterp(const Quat<T> &, const Quat<T> &, U);
	///@}

	/*--------------------+
	| cubic interpolation |
	+--------------------*/

	/**
	 * @defgroup
	 * A standard entry-point for cubic interpolation, which delegates to the
	 * most appropriate implementation for the type.
	 * @{
	 */
	template <typename T, typename U>
		T CubicInterp(T, T, T, T, U);
	template <typename T, typename U>
		Quat<T> CubicInterp(const Quat<T> &, const Quat<T> &, const Quat<T> &, const Quat<T> &, U);
	///@}

	/*-------------------------+
	| non-linear interpolation |
	+-------------------------*/

	/**
	 * Hermite-spline interpolation.
	 */
	template <typename T, typename U>
		T HermiteInterp(T, T, U);

	/**
	 * Hermite-spline interpolation, with an exponent.
	 */
	template <typename T, typename U, typename V>
		T HermiteInterp(T, T, U, V exp);

	/*-------------------+
	| non-linear scaling |
	+-------------------*/

	/**
	 * Hermite-spline scaling.
	 */
	template <typename T>
		T HermiteScale(T);

	/**
	 * Hermite-spline scaling, with an exponent.
	 */
	template <typename T, typename U>
		T HermiteScale(T, U exp);

	/*---------------------------+
	| convolution-filter kernels |
	+---------------------------*/

	/**
	 * Generates a hermite-spline convolution-kernel.
	 */
	template <unsigned n, typename T>
		std::array<T, n> HermiteConvolutionKernel(T size);

	/**
	 * Generates a hermite-spline convolution-kernel, using an exponent.
	 */
	template <unsigned n, typename T, typename U>
		std::array<T, n> HermiteConvolutionKernel(T size, U exp);
}}

#	include "interp.tpp"
#endif
