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

#ifndef    page_math_interp_hpp
#   define page_math_interp_hpp

#	include <array>
#	include "Quat.hpp"

namespace page
{
	namespace math
	{
		// standard interpolation
		template <typename T, typename U> T Lerp(T, T, U);
		template <typename T, typename U> Quat<T> Lerp(const Quat<T> &, const Quat<T> &, U);
		template <typename T, typename U> Quat<T> Slerp(const Quat<T> &, const Quat<T> &, U);
		template <typename T, typename U> Quat<T> FastSlerp(const Quat<T> &, const Quat<T> &, U);
		template <typename T, typename U> Quat<T> Squad(const Quat<T> &, const Quat<T> &, const Quat<T> &, const Quat<T> &, U);
		template <typename T, typename U> T Bilerp(T, T, T, U, U);
		template <typename T, typename U> Quat<T> Bilerp(const Quat<T> &, const Quat<T> &, const Quat<T> &, U, U);

		// linear imterpolation
		template <typename T, typename U> T LinearInterp(T, T, U);
		template <typename T, typename U> Quat<T> LinearInterp(const Quat<T> &, const Quat<T> &, U);

		// cubic interpolation
		template <typename T, typename U> T CubicInterp(T, T, T, T, U);
		template <typename T, typename U> Quat<T> CubicInterp(const Quat<T> &, const Quat<T> &, const Quat<T> &, const Quat<T> &, U);

		// non-linear interpolation
		template <typename T, typename U> T HermiteInterp(T, T, U);
		template <typename T, typename U, typename V> T HermiteInterp(T, T, U, V exp);

		// non-linear scaling
		template <typename T> T HermiteScale(T);
		template <typename T, typename U> T HermiteScale(T, U exp);

		// convolution-filter kernels
		template <unsigned n, typename T> std::array<T, n> HermiteConvolutionKernel(T size);
		template <unsigned n, typename T, typename U> std::array<T, n> HermiteConvolutionKernel(T size, U exp);
	}
}

#	include "interp.tpp"
#endif
