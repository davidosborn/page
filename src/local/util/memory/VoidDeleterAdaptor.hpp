/**
 * @section license
 *
 * Copyright (c) 2006-2013 David Osborn
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

#ifndef    page_local_util_memory_VoidDeleterAdaptor_hpp
#   define page_local_util_memory_VoidDeleterAdaptor_hpp

#	include <functional> // function

namespace page { namespace util
{
	/**
	 * An adaptor to allow a deleter taking a typed pointer to be stored in a
	 * pointer to a function taking a @c void pointer.
	 */
	template <typename T>
		struct VoidDeleterAdaptor
	{
		typedef void result_type;
		typedef const T *argument_type;

		private:
		typedef std::function<void (const T *)> deleter_type;

		public:
		explicit VoidDeleterAdaptor(const deleter_type &deleter) :
			deleter(deleter) {}

		void operator ()(const void *x) const
		{
			return deleter(static_cast<const T *>(x));
		}

		private:
		deleter_type deleter;
	};
}}

#endif
