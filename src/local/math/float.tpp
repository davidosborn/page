#include <cassert>
#include <cmath> // a{cos,tan}, copysign, cos, exp, fmod, is{inf,nan}, log10, nexttoward, pow, sqrt
#include <cstdint> // int{32,64}_t
#include <cstdlib> // abs
#include <limits> // numeric_limits

#include "ArithmeticConversion.hpp"

namespace page { namespace math
{
	// HACK: higher tolerance to support -ffast-math
	const unsigned tolerance = 16; // equality comparison tolerance

	namespace detail
	{
		/// tolerant equality comparison implementations
		template <typename T> inline bool Near(T a, T b)
		{
			if (a == b) return true;
			for (unsigned i = 0; i < tolerance; ++i)
				if ((a = std::nexttoward(a, b)) == b) return true;
			return false;
		}
		// based on Bruce Dawson's "Comparing floating point numbers"
		// http://www.cygnus-software.com/papers/comparingfloats/comparingfloats.htm
		template <> inline bool Near(float a, float b)
		{
			// HACK: this only works with IEEE 754 single-precision types
			assert(sizeof(float) == sizeof(std::int32_t));
			if (a == b) return true;
			union { float f; std::int32_t i; } au = {a}, bu = {b};
			if (au.i < 0) au.i = (std::int32_t(1) << 31) - au.i;
			if (bu.i < 0) bu.i = (std::int32_t(1) << 31) - bu.i;
			return std::abs(au.i - bu.i) <= tolerance;
		}
		template <> inline bool Near(double a, double b)
		{
			// HACK: this only works with IEEE 754 double-precision types
			// HACK: signed left shift is assumed to destroy sign bit
			assert(sizeof(double) == sizeof(std::int64_t));
			if (a == b) return true;
			union { double f; std::int64_t i; } au = {a}, bu = {b};
			if (au.i < 0) au.i = (std::int64_t(1) << 63) - au.i;
			if (bu.i < 0) bu.i = (std::int64_t(1) << 63) - bu.i;
			return std::abs(au.i - bu.i) <= tolerance;
		}
	}

	/// special constants
	template <typename T> T Inf()
	{
		static const T inf =
			std::numeric_limits<T>::has_infinity ?
			std::numeric_limits<T>::infinity() :
			std::numeric_limits<T>::max();
		return inf;
	}
	template <typename T> T Nan()
	{
		static const T nan =
			std::numeric_limits<T>::has_quiet_NaN ?
			std::numeric_limits<T>::quiet_NaN() :
			std::numeric_limits<T>::has_signaling_NaN ?
			std::numeric_limits<T>::signaling_NaN() :
			std::numeric_limits<T>::max();
		return nan;
	}
	template <typename T> T Pi()
	{
		static const T pi = std::atan(T(1)) * 4;
		return pi;
	}
	template <typename T> T E()
	{
		static const T e = std::exp(T(1));
		return e;
	}
	template <typename T> T Phi()
	{
		static const T phi = std::sqrt(T(5)) + 1 / 2;
		return phi;
	}

	/// special constant comparisons
	template <typename T> bool IsInf(T t)
	{
		return
			std::numeric_limits<T>::has_infinity ?
			std::isinf(t) : t == std::copysign(Inf<T>(), t);
	}
	template <typename T> bool IsNan(T t)
	{
		return
			std::numeric_limits<T>::has_quiet_NaN ||
			std::numeric_limits<T>::has_signaling_NaN ?
			std::isnan(t) : t == std::copysign(Nan<T>(), t);
	}

	/// tolerant equality comparison
	template <typename T, typename U> bool Near(T a, U b)
	{
		typedef typename ArithmeticConversion<T, U>::Result R;
		return detail::Near<R>(a, b);
	}

	/// quantization
	template <typename T> T Round(T t)
	{
		return std::floor(t + T(.5));
	}
	template <typename T, typename U> typename ArithmeticConversion<T, U>::Result Round(T t, U precision)
	{
		return Round(t / precision) * precision;
	}

	/// wrapping
	template <typename T, typename U>
		typename ArithmeticConversion<T, U>::Result Wrap(T t, U min, U max)
	{
		U range = max - min;
		return std::fmod(std::fmod(t - min, range) + range, range) + min;
	}

	/// angle conversions
	template <typename T> T DegToRad(T t) { return t * Pi<T>() / 180; }
	template <typename T> T DegToRev(T t) { return t / 360; }
	template <typename T> T RadToDeg(T t) { return t * 180 / Pi<T>(); }
	template <typename T> T RadToRev(T t) { return t / (Pi<T>() * 2); }
	template <typename T> T RevToDeg(T t) { return t * 360; }
	template <typename T> T RevToRad(T t) { return t * (Pi<T>() * 2); }

	/// angle calculations
	template <typename T> T AngleGivenSides(T a, T b, T c)
	{
		return std::acos((a * a + b * b - c * c) / (a * b * 2));
	}
	template <typename T> T SideGivenSidesAngle(T a, T b, T c)
	{
		return std::sqrt(a * a + b * b - a * b * 2 * std::cos(c));
	}
	template <typename T> T FirstAngleGivenSidesAngle(T a, T b, T c)
	{
		return AngleGivenSides(SideGivenSidesAngle(a, b, c), a, b);
	}
	template <typename T> T SecondAngleGivenSidesAngle(T a, T b, T c)
	{
		return AngleGivenSides(SideGivenSidesAngle(a, b, c), b, a);
	}

	/// volume conversions
	template <typename T> T DecibelToLinear(T t)
	{
		return std::pow(10, t / 20);
	}
	template <typename T> T LinearToDecibel(T t)
	{
		return std::log10(t) * 20;
	}
}}
