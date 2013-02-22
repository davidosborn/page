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

#ifndef    page_local_util_memory_Deleter_hpp
#   define page_local_util_memory_Deleter_hpp

#	include <memory> // shared_ptr

namespace page
{
	namespace util
	{
		/**
		 * A function object containing a generic deleter, for use with
		 * @c shared_ptr and @c unique_ptr.
		 */
		typedef std::function<void (const void *)> Deleter;

		/**
		 * A default deleter.
		 *
		 * @note This deleter takes a pointer-to-void to provide compatibility
		 *       with @c Deleter.
		 */
		template <typename T>
			struct DefaultDeleter
		{
			typedef const void *argument_type;
			typedef void result_type;

			void operator ()(const void *p) const
			{
				delete static_cast<const T *>(p);
			}
		};

		/**
		 * A no-op deleter.
		 */
		struct NopDeleter
		{
			typedef const void *argument_type;
			typedef void result_type;

			void operator ()(const void *) const {}
		};

		/**
		 * @return A @c std::shared_ptr representation of a static instance.
		 */
		template <typename T>
			std::shared_ptr<T> Share(T &p)
		{
			return std::shared_ptr<T>(&p, NopDeleter());
		}
	}
}

#endif
