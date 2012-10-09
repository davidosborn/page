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

#include <algorithm> // max, min, transform
#include <cassert>
#include <cmath> // abs, acos, copysign, cos, pow, sin
#include <functional> // bind, divides
#include <numeric> // accumulate

namespace page
{
	namespace math
	{
		// standard interpolation
		template <typename T, typename U> T Lerp(T a, T b, U mu)
		{
			return a + (b - a) * mu;
		}
		template <typename T, typename U> Quat<T> Lerp(const Quat<T> &a, const Quat<T> &b, U mu)
		{
			Quat<T> a2(a);
			// FIXME: don't need this check if called from Slerp or FastSlerp
			if (Dot(a, b) < 0) a2 = -a2;
			return Norm(a2 + (b - a2) * mu);
		}
		template <typename T, typename U> Quat<T> Slerp(const Quat<T> &a, const Quat<T> &b, U mu)
		{
			// Van Verth, Essential Mathematics for Games, 2004
			typedef typename ArithmeticConversion<T, U>::Result R;
			Quat<T> a2(a);
			T dot = Dot(a, b);
			if (dot < 0)
			{
				a2 = -a2;
				dot = -dot;
			}
			if (dot > .999f) return Lerp(a2, b, mu);
			T th = std::acos(dot);
			return (a2 * std::sin((1 - mu) * th) + b * std::sin(mu * th)) / std::sin(th);
		}
		template <typename T, typename U> Quat<T> FastSlerp(const Quat<T> &a, const Quat<T> &b, U mu)
		{
			// Van Verth, Essential Mathematics for Games, 2004
			// Jonathan Blow, Hacking Quaternions, Game Developer, March 2002
			Quat<T> a2(a);
			T dot = Dot(a, b);
			if (dot < 0) a2 = -a2;
			T
				factor = 1 - dot * .7878088f,
				k = factor * factor * .5069269f,
				mu2 = (k * 2 * mu - k * 3) * mu + k + 1;
			return Lerp(a2, b, mu2);
		}
		template <typename T, typename U> Quat<T> Squad(const Quat<T> &a, const Quat<T> &b, const Quat<T> &c, const Quat<T> &d, U mu)
		{
			return Slerp(Slerp(a, d, mu), Slerp(b, c, mu), 2 * (1 - mu) * mu);
		}
		template <typename T, typename U> T Bilerp(T a, T b, T c, U mu1, U mu2)
		{
			return a + (b - a) * mu1 + (c - a) * mu2;
		}
		template <typename T, typename U> Quat<T> Bilerp(const Quat<T> &a, const Quat<T> &b, const Quat<T> &c, U mu1, U mu2)
		{
			return Norm(a + (b - a) * mu1 + (c - a) * mu2);
		}

		// linear interpolation
		template <typename T, typename U> T LinearInterp(T a, T b, U mu)
		{
			return Lerp(a, b, mu);
		}
		template <typename T, typename U> Quat<T> LinearInterp(const Quat<T> &a, const Quat<T> &b, U mu)
		{
			return Slerp(a, b, mu);
		}

		// cubic interpolation
		template <typename T, typename U> T CubicInterp(T a, T b, T c, T d, U mu)
		{
			T
				p = d - c - (a - b),
				q = a - b - p,
				r = c - a;
			return ((p * mu + q) * mu + r) * mu + b;
		}
		template <typename T, typename U> Quat<T> CubicInterp(const Quat<T> &a, const Quat<T> &b, const Quat<T> &c, const Quat<T> &d, U mu)
		{
			return Squad(a, b, c, d, mu);
		}

		// non-linear interpolation
		template <typename T, typename U> T HermiteInterp(T a, T b, U mu)
		{
			assert(mu >= 0 && mu <= 1);
			// based on smoothstep as defined in the GLSL Specification
			return a + (b - a) * mu * mu * (3 - 2 * mu);
		}
		template <typename T, typename U, typename V> T HermiteInterp(T a, T b, U mu, V exp)
		{
			assert(mu >= 0 && mu <= 1);
			// based on smoothstep as defined in the GLSL Specification
			U
				c = mu * mu * (6 - 4 * mu) - 1,
				d = 1 - std::pow(1 - std::abs(c), exp);
			return a + (b - a) * (std::copysign(d, c) * .5f + .5f);
		}

		// non-linear scaling
		template <typename T> T HermiteScale(T mu)
		{
			return HermiteInterp<T>(0, 1, mu);
		}
		template <typename T, typename U> T HermiteScale(T mu, U exp)
		{
			return HermiteInterp<T>(0, 1, mu, exp);
		}

		// convolution-filter kernels
		template <unsigned n, typename T> std::array<T, n> HermiteConvolutionKernel(T size)
		{
			static_assert(n % 2, "kernel size must be odd");
			typedef std::array<T, n> Kernel;
			Kernel kernel;
			T inc = 1 / size, mu = -int(n / 2) * inc;
			for (typename Kernel::iterator iter(kernel.begin()); iter != kernel.end(); ++iter)
			{
				*iter = HermiteInterp<T>(1, 0, std::min(std::max(std::abs(mu), T(0)), T(1)));
				mu += inc;
			}
			std::transform(kernel.begin(), kernel.end(), kernel.begin(),
				std::bind(std::divides<T>(), std::placeholders::_1,
					std::accumulate(kernel.begin(), kernel.end(), T(0))));
			return kernel;
		}
		template <unsigned n, typename T, typename U> std::array<T, n> HermiteConvolutionKernel(T size, U exp)
		{
			static_assert(n % 2, "kernel size must be odd");
			typedef std::array<T, n> Kernel;
			Kernel kernel;
			T inc = 1 / size, mu = -int(n / 2) * inc;
			for (typename Kernel::iterator iter(kernel.begin()); iter != kernel.end(); ++iter)
			{
				*iter = HermiteInterp<T>(1, 0, std::min(std::max(std::abs(mu), T(0)), T(1)), exp);
				mu += inc;
			}
			std::transform(kernel.begin(), kernel.end(), kernel.begin(),
				std::bind(std::divides<T>(), std::placeholders::_1,
					std::accumulate(kernel.begin(), kernel.end(), T(0))));
			return kernel;
		}
	}
}
