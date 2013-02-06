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

#include <utility> // move

namespace page
{
	namespace util
	{
		// deleter
		template <typename T> struct unique_ptr<T>::ConstDeleterAdapter
		{
			explicit ConstDeleterAdapter(deleter_type deleter) :
				deleter(deleter) {}

			void operator ()(const T *p) const
			{
				deleter(const_cast<T *>(p));
			}

			private:
			deleter_type deleter;
		};

		// constructors
		template <typename T> unique_ptr<T>::unique_ptr() : Base() {}
		template <typename T> unique_ptr<T>::unique_ptr(pointer p) : Base(p) {}
		template <typename T> unique_ptr<T>::unique_ptr(pointer p, deleter_type deleter) : Base(p, deleter) {}
		template <typename T> unique_ptr<T>::unique_ptr(std::nullptr_t) : Base(nullptr) {}
		template <typename T> unique_ptr<T>::unique_ptr(unique_ptr &&other) : Base(std::move(other)) {}
		template <typename T> template <typename U> unique_ptr<T>::unique_ptr(std::auto_ptr<U> &&other) : Base(std::move(other)) {}
		template <typename T> template <typename U, typename D> unique_ptr<T>::unique_ptr(std::unique_ptr<U, D> &&other) : Base(std::move(other)) {}

		// destructor
		template <typename T> unique_ptr<T>::~unique_ptr()
		{
			deleter_type deleter(this->get_deleter());
			if (deleter != nullptr) deleter(this->get());
			this->release();
		}

		// observers
		template <typename T> typename unique_ptr<T>::const_deleter_type unique_ptr<T>::get_const_deleter() const
		{
			return ConstDeleterAdapter(this->get_deleter());
		}
	}
}
