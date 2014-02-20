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

#ifndef    page_local_util_memory_DefaultDeleter_hpp
#   define page_local_util_memory_DefaultDeleter_hpp

namespace page { namespace util
{
	/**
	 * The default implementation of a deleter.
	 */
	template <typename T>
		struct DefaultDeleter
	{
		typedef void result_type;
		typedef const T *argument_type;

		void operator ()(const T *x) const
		{
			delete x;
		}
	};

	/**
	 * The default implementation of a non-const deleter.  This is the kind of
	 * deleter used by @c std::unique_ptr.
	 */
	template <typename T>
		struct DefaultNonConstDeleter
	{
		typedef void result_type;
		typedef T *argument_type;

		void operator ()(T *x) const
		{
			delete x;
		}
	};
}}

#endif
