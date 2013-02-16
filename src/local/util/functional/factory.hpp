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

#ifndef    page_local_util_functional_factory_hpp
#   define page_local_util_functional_factory_hpp

#	include <utility> // forward

namespace page
{
	namespace util
	{
		/**
		 * @defgroup functional-factory
		 *
		 * Function objects for operators that are missing from the C++
		 * Standard Library.
		 *
		 * @{
		 */
		/**
		 * A function object that wraps @c operator @c new.
		 */
		template <typename T, typename... Args>
			struct new_function
		{
			typedef T *result_type;
			typedef result_type signature(Args...);

			T *operator ()(Args &&... args) const
			{
				return new T(std::forward<Args>(args)...);
			}
		};

		/**
		 * A function object that wraps @c operator @c new and returns a
		 * @c std::unique_ptr.
		 */
		template <typename T, typename... Args>
			struct factory_function
		{
			typedef std::unique_ptr<T> result_type;
			typedef result_type signature(Args...);

			std::unique_ptr<T> operator ()(Args &&... args) const
			{
				return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
			}
		};

		/**
		 * A function object that wraps @c std::make_shared.
		 */
		template <typename T, typename... Args>
			struct make_shared_function
		{
			typedef std::shared_ptr<T> result_type;
			typedef result_type signature(Args...);

			std::shared_ptr<T> operator ()(Args &&... args) const
			{
				return std::make_shared<T>(std::forward<Args>(args)...);
			}
		};
		
		/**
		 * A function object that creates an object on the stack.
		 */
		template <typename T, typename... Args>
			struct constructor_function
		{
			typedef T result_type;
			typedef result_type signature(Args...);

			T operator ()(Args &&... args) const
			{
				return T(std::forward<Args>(args)...);
			}
		};
		///@}
	}
}

#endif
