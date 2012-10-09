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

#ifndef    page_util_functional_hpp
#   define page_util_functional_hpp

#	include <cmath> // fmod
#	include <locale> // isspace, locale, to{lower,upper}
#	include <memory> // addressof
#	include <type_traits> // is_floating_point
#	include <utility> // forward
#	include "type_traits.hpp" // remove_indirection

namespace page
{
	namespace util
	{
		/**
		 * @defgroup functional Functional
		 *
		 * @{
		 */
		/**
		 * @class unary_plus
		 *
		 * Provided for symmetry with @c std::negate.
		 */
		template <typename T> struct unary_plus
		{
			T operator ()(T a) const { return +a; }
		};

		/**
		 * @defgroup functional-modulus Functional modulus
		 *
		 * @{
		 */
		namespace detail
		{
			/**
			 * @defgroup functional-modulus-impl Functional modulus implementation
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
				T operator ()(const T &a, const T &b) const { return a % b; }
			};
			/**
			 * A function-object wrapper around @c std::fmod.
			 */
			template <typename T>
				struct modulus_impl<T, true>
			{
				T operator ()(T a, T b) const { return std::fmod(a, b); }
			};
			///@}
		}
		/**
		 * Like @c std::modulus, but with a specialization for floating-point.
		 */
		template <typename T> struct modulus : detail::modulus_impl<T> {};
		///@}

		/**
		 * @defgroup functional-bitwise-shift Functional bitwise-shifting
		 *
		 * Function objects for performing bitwise shifts.
		 *
		 * @{
		 */
		/**
		 * A function object for bitwise left-shift.
		 */
		template <typename T> struct bit_shl
		{
			T operator ()(const T &a, unsigned n) const { return a << n; }
		};
		/**
		 * A function object for bitwise right-shift.
		 */
		template <typename T> struct bit_shr
		{
			T operator ()(const T &a, unsigned n) const { return a >> n; }
		};
		///@}
		
		/**
		 * @defgroup functional-pointer-manip Functional pointer manipulation
		 *
		 * Function objects for manipulating pointers.
		 *
		 * @{
		 */
		/**
		 * A function object for dereferencing pointers or pointer-like objects.
		 */
		template <typename T> struct dereference
		{
			typename util::remove_indirection<T>::reference
				operator ()(const T &a) const { return *a; }
		};
		/**
		 * A function object for applying the address-of operator.
		 *
		 * @note Not to be confused with @c std::addressof, which finds the real
		 *       address of an object, regardless of any overloading of the
		 *       address-of operator.
		 */
		template <typename T> struct address_of
		{
			T *operator ()(T &a) const { return &a; }
		};
		/**
		 * A function-object wrapper around @c std::addressof.
		 */
		template <typename T> struct real_address_of
		{
			T *operator ()(T &a) const { return std::addressof(a); }
		};
		///@}

		/**
		 * @defgroup functional-member-extraction Functional member extraction
		 *
		 * Functional tools for extracting members.
		 *
		 * @{
		 */
		/**
		 * A function object for extracting a member of an aggregate.
		 */
		template <typename T, typename M> struct member_of
		{
			explicit member_of(M T::*m) : m(m) {}

			M &operator ()(T &t) const {return t.*m;}
			const M &operator ()(const T &t) const {return t.*m;}

			private:
			M T::*m;
		};
		/**
		 * A convenience function for creating instances of @c member_of.
		 */
		template <typename T, typename M> member_of<T, M> make_member_of(M T::*m)
		{
			return member_of<T, M>(m);
		}
		///@}

		/**
		 * @defgroup functional-cast Functional type-casting
		 *
		 * Functional wrappers around type-casting operators.
		 *
		 * @{
		 */
		template <typename T, typename U> struct const_cast_function
		{
			T operator ()(U t) const { return const_cast<T>(t); }
		};
		template <typename T, typename U> struct dynamic_cast_function
		{
			T operator ()(U t) const { return dynamic_cast<T>(t); }
		};
		template <typename T, typename U> struct reinterpret_cast_function
		{
			T operator ()(U t) const { return reinterpret_cast<T>(t); }
		};
		template <typename T, typename U> struct static_cast_function
		{
			T operator ()(U t) const { return static_cast<T>(t); }
		};
		///@}
		
		/**
		 * @defgroup functional-locale Functional localization
		 *
		 * Functional wrappers around localization and character-classification
		 * functions from the @c <locale> and @c <cctype> headers.
		 *
		 * @note Lambda expressions should generally be preferred to these.
		 *
		 * @{
		 */
		template <typename Char> struct isnewline_function
		{
			bool operator ()(Char c) const
			{
				switch (c)
				{
					case '\n':
					case '\r': return true;
					default:   return false;
				}
			}
		};
		template <typename Char> struct isspace_function
		{
			bool operator ()(Char c) const
			{
				return std::isspace(c, std::locale());
			}
		};
		template <typename Char> struct tolower_function
		{
			bool operator ()(Char c) const
			{
				return std::tolower(c, std::locale());
			}
		};
		template <typename Char> struct toupper_function
		{
			bool operator ()(Char c) const
			{
				return std::tolower(c, std::locale());
			}
		};
		///@}
		
		/**
		 * Functional wrapper around @c operator @c new.
		 */
		template <typename T, typename... Args> struct new_function
		{
			T *operator ()(Args &&... args) const
			{
				return new T(std::forward<Args>(args)...);
			}
		};
		///@}
	}
}

#endif
