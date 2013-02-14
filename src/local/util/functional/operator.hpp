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

#ifndef    page_local_util_functional_operator_hpp
#   define page_local_util_functional_operator_hpp

#	include <cmath> // fmod
#	include <type_traits> // is_floating_point

namespace page
{
	namespace util
	{
		/**
		 * @defgroup functional-operator
		 *
		 * Function objects for operators that are missing from the C++
		 * Standard Library.
		 *
		 * @{
		 */
		/**
		 * A function object for bitwise left-shift.
		 */
		template <typename T>
			struct bit_shl
		{
			T operator ()(const T &a, unsigned n) const
			{
				return a << n;
			}
		};

		/**
		 * A function object for bitwise right-shift.
		 */
		template <typename T>
			struct bit_shr
		{
			T operator ()(const T &a, unsigned n) const
			{
				return a >> n;
			}
		};

		/**
		 * A function object that wraps the unary-plus operator.
		 *
		 * @note Provided for symmetry with @c std::negate.
		 */
		template <typename T>
			struct unary_plus
		{
			T operator ()(T a) const
			{
				return +a;
			}
		};
		///@}

		/**
		 * @defgroup functional-modulus
		 *
		 * @{
		 */
		namespace detail
		{
			/**
			 * @defgroup functional-modulus-impl
			 * @ingroup  functional-modulus
			 *
			 * @{
			 */
			template <typename T, bool = std::is_floating_point<T>::value>
				class modulus_impl;

			/**
			 * Equivalent to @c std::modulus.
			 */
			template <typename T>
				struct modulus_impl<T, false>
			{
				T operator ()(const T &a, const T &b) const
				{
					return a % b;
				}
			};

			/**
			 * A function object that wraps @c std::fmod.
			 */
			template <typename T>
				struct modulus_impl<T, true>
			{
				T operator ()(T a, T b) const
				{
					return std::fmod(a, b);
				}
			};
			///@}
		}

		/**
		 * Like @c std::modulus, but with a specialization for floating-point.
		 */
		template <typename T>
			struct modulus : detail::modulus_impl<T> {};
		///@}
	}
}

#endif
