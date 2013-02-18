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

#ifndef    page_local_util_type_traits_const_volatile_hpp
#   define page_local_util_type_traits_const_volatile_hpp

#	include <type_traits>

namespace page
{
	namespace util
	{
		/**
		 * @defgroup copy_const
		 * @{
		 */
		namespace detail
		{
			template <typename To, typename From, bool = std::is_const<From>::value>
				struct copy_const_impl
			{
				typedef typename std::add_const<To>::type type;
			};
			template <typename To, typename From>
				struct copy_const_impl<To, From, false>
			{
				typedef typename std::remove_const<To>::type type;
			};
		}

		/**
		 * Copies const qualifiers from one type to another.
		 */
		template <typename To, typename From>
			struct copy_const
		{
			typedef typename detail::copy_const_impl<To, From>::type type;
		};
		///@}

		/**
		 * @defgroup copy_volatile
		 * @{
		 */
		namespace detail
		{
			template <typename To, typename From, bool = std::is_volatile<From>::value>
				struct copy_volatile_impl
			{
				typedef typename std::add_volatile<To>::type type;
			};
			template <typename To, typename From>
				struct copy_volatile_impl<To, From, false>
			{
				typedef typename std::remove_volatile<To>::type type;
			};
		}

		/**
		 * Copies volatile qualifiers from one type to another.
		 */
		template <typename To, typename From>
			struct copy_volatile
		{
			typedef typename detail::copy_volatile_impl<To, From>::type type;
		};
		///@}

		/**
		 * @defgroup copy_cv
		 * @{
		 */
		/**
		 * Copies const/volatile qualifiers from one type to another.
		 */
		template <typename To, typename From>
			struct copy_cv
		{
			typedef
				typename copy_const<
				typename copy_volatile<To, From>::type, From>::type type;
		};
		///@}
	}
}

#endif
