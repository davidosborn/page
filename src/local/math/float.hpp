#ifndef    page_math_float_hpp
#   define page_math_float_hpp

#	include <functional> // binary_function
#	include <limits> // numeric_limits

#	include "ArithmeticConversion.hpp"
#	include "fwd.hpp" // DefaultType

namespace page { namespace math
{
	/// type conversion
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

	/// special constants
	template <typename T = float> T Inf();
	template <typename T = float> T Nan();
	template <typename T = float> T Pi();
	template <typename T = float> T E();
	template <typename T = float> T Phi(); // golden ratio

	/// special constant comparisons
	template <typename T> bool IsInf(T);
	template <typename T> bool IsNan(T);

	/// tolerant equality comparison
	template <typename T, typename U> bool Near(T, U);

	/// quantization
	template <typename T> T Round(T);
	template <typename T, typename U>
		typename ArithmeticConversion<T, U>::Result Round(T, U precision);

	/// wrapping
	template <typename T, typename U>
		typename ArithmeticConversion<T, U>::Result Wrap(T, U min, U max);

	/// angle conversions
	template <typename T> T DegToRad(T);
	template <typename T> T DegToRev(T);
	template <typename T> T RadToDeg(T);
	template <typename T> T RadToRev(T);
	template <typename T> T RevToDeg(T);
	template <typename T> T RevToRad(T);

	/// angle calculations
	template <typename T> T AngleGivenSides(T, T, T);
	template <typename T> T SideGivenSidesAngle(T, T, T);
	template <typename T> T FirstAngleGivenSidesAngle(T, T, T);
	template <typename T> T SecondAngleGivenSidesAngle(T, T, T);

	/// audio-volume conversions
	template <typename T> T DecibelToLinear(T);
	template <typename T> T LinearToDecibel(T);
}}

#	include "float.tpp"
#endif
