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

#ifndef    page_local_math_ArithmeticConversion_hpp
#   define page_local_math_ArithmeticConversion_hpp

#	include <utility> // declval

namespace page
{
	namespace math
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
			 * A specialization of the implementation of @c ArithmeticConversion
			 * for when one or both of the types are not arithmetic/enum types.
			 * In this case, the arithmetic conversion cannot be performed and
			 * the resulting type defaults to void.
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
	}
}

#endif
