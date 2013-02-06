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

#ifndef    page_local_util_unique_ptr_hpp
#   define page_local_util_unique_ptr_hpp

#	include <cstddef> // nullptr_t
#	include <functional> // function
#	include <memory> // {auto,unique}_ptr

namespace page
{
	namespace util
	{
		/**
		 * A modified version of @c std::unique_ptr, with a hard-coded deleter
		 * type.
		 *
		 * @note This class supports incomplete types.
		 * @note The deleter may be initialized to @c nullptr on construction,
		 *       in which case it will not be executed on destruction.
		 *
		 * @ingroup smart-pointer
		 */
		template <typename T> struct unique_ptr :
			std::unique_ptr<T, std::function<void (T *)>>
		{
			typedef std::unique_ptr<T, std::function<void (T *)>> Base;
			typedef typename Base::pointer pointer;
			typedef typename Base::element_type element_type;
			typedef typename Base::deleter_type deleter_type;
			typedef std::function<void (const T *)> const_deleter_type;
		
			// constructors
			unique_ptr();
			explicit unique_ptr(pointer);
			unique_ptr(pointer, deleter_type);
			unique_ptr(std::nullptr_t);
			unique_ptr(unique_ptr &&);
			template <typename U> unique_ptr(std::auto_ptr<U> &&);
			template <typename U, typename D> unique_ptr(std::unique_ptr<U, D> &&);
			
			// destructor
			~unique_ptr();

			// observers
			const_deleter_type get_const_deleter() const;
			
			// disable copy from lvalue
			unique_ptr(const unique_ptr &) = delete;
			unique_ptr &operator =(const unique_ptr &) = delete;
			
			private:
			// deleter
			class ConstDeleterAdapter;
		};
	}
}

#	include "unique_ptr.tpp"
#endif
