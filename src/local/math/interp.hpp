#ifndef    page_math_interp_hpp
#   define page_math_interp_hpp

#	include <array>

#	include "fwd.hpp" // Quat

namespace page { namespace math
{
	/// standard interpolation
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

	/// linear interpolation
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

	/// cubic interpolation
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

	/// non-linear interpolation
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

	/// non-linear scaling
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

	/// convolution-filter kernels
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
