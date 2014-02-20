/**
 * @copyright
 *
 * Copyright (c) 2006-2014 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the same place
 *    and form as other copyright, license, and disclaimer information.
 *
 * 3. Redistributions in binary form must also include an acknowledgement in the
 *    same place and form as other acknowledgements (such as the credits),
 *    similar in substance to the following:
 *
 *       Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#ifndef    page_local_util_functional_pointer_hpp
#   define page_local_util_functional_pointer_hpp

#	include <memory> // addressof

#	include "../type_traits/pointer.hpp" // remove_indirection

namespace page
{
	namespace util
	{
		/**
		 * @defgroup functional-pointer
		 *
		 * Function objects for manipulating pointers.
		 *
		 * @{
		 */
		/**
		 * A function object for dereferencing pointers or pointer-like objects.
		 */
		template <typename T>
			struct dereference
		{
			typename util::remove_indirection<T>::reference
				operator ()(const T &a) const
			{
				return *a;
			}
		};

		/**
		 * A function object for applying the address-of operator.
		 *
		 * @note Not to be confused with @c std::addressof, which finds the real
		 *       address of an object, regardless of any overloading of the
		 *       address-of operator.
		 */
		template <typename T>
			struct address_of
		{
			T *operator ()(T &a) const
			{
				return &a;
			}
		};

		/**
		 * A function-object wrapper around @c std::addressof.
		 */
		template <typename T>
			struct real_address_of
		{
			T *operator ()(T &a) const
			{
				return std::addressof(a);
			}
		};
		///@}
	}
}

#endif
