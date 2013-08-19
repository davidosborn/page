#ifndef    page_math_ArithmeticConversion_hpp
#   define page_math_ArithmeticConversion_hpp

#	include <utility> // declval

namespace page { namespace math
{
	namespace detail
	{
		/**
		 * The implementation of @c ArithmeticConversion.
		 */
		template <typename L, typename R, bool =
			(std::is_arithmetic<L>::value || std::is_enum<L>::value) &&
			(std::is_arithmetic<R>::value || std::is_enum<R>::value)> class ArithmeticConversionImpl;
		template <typename L, typename R> struct ArithmeticConversionImpl<L, R, true>
		{
			typedef decltype(std::declval<L>() + std::declval<R>()) Result;
		};
		/**
		 * A specialization of the implementation of @c ArithmeticConversion for
		 * when one or both of the types are not arithmetic/enum types. In this
		 * case, the arithmetic conversion cannot be performed and the resulting
		 * type defaults to void.
		 */
		template <typename L, typename R> struct ArithmeticConversionImpl<L, R, false>
		{
			typedef void Result;
		};
	}

	/**
	 * Determine the type resulting from a "usual arithmetic conversion"
	 * (ISO/IEC 14882:2003 5.1.9)
	 */
	template <typename L, typename R> struct ArithmeticConversion :
		detail::ArithmeticConversionImpl<L, R> {};
}}

#endif
