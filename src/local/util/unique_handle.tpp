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

namespace page
{
	namespace util
	{
		// default deleter
		template <typename T> struct unique_handle<T>::DefaultDeleter
		{
			void operator ()(typename unique_handle<T>::handle_type handle_type) const {}
		};
		
		// constructors
		template <typename T> unique_handle<T>::unique_handle() :
			unique_handle(nullptr) {}
		template <typename T> unique_handle<T>::unique_handle(handle_type handle) :
			unique_handle(handle, DefaultDeleter()) {}
		template <typename T> unique_handle<T>::unique_handle(handle_type handle, deleter_type deleter) :
			handle(handle), deleter(deleter) {}
		template <typename T> unique_handle<T>::unique_handle(std::nullptr_t) :
			unique_handle(nullptr, DefaultDeleter()) {}
		template <typename T> unique_handle<T>::unique_handle(std::nullptr_t, deleter_type deleter) :
			handle(), deleter(deleter) {}
		template <typename T> unique_handle<T>::unique_handle(unique_handle &&other) :
			handle(other.handle), deleter(other.deleter) {}
		template <typename T> template <typename U> unique_handle<T>::unique_handle(unique_handle<U> &&other) :
			handle(other.handle), deleter(other.deleter) {}

		// destructor
		template <typename T> unique_handle<T>::~unique_handle()
		{
			if (handle && deleter) deleter(*handle);
		}

		// assignment
		template <typename T> unique_handle<T> &unique_handle<T>::operator =(unique_handle &&other)
		{
			unique_handle(other).swap(*this);
			return *this;
		}
		template <typename T> template <typename U> unique_handle<T> &unique_handle<T>::operator =(unique_handle<U> &&other)
		{
			unique_handle(other).swap(*this);
			return *this;
		}
		
		// observers
		template <typename T> typename unique_handle<T>::handle_type &unique_handle<T>::unique_handle::operator *() noexcept
		{
			assert(handle);
			return *handle;
		}
		template <typename T> const typename unique_handle<T>::handle_type &unique_handle<T>::unique_handle::operator *() const noexcept
		{
			assert(handle);
			return *handle;
		}
		template <typename T> typename unique_handle<T>::handle_type *unique_handle<T>::unique_handle::operator ->() noexcept
		{
			assert(handle);
			return handle.get();
		}
		template <typename T> const typename unique_handle<T>::handle_type *unique_handle<T>::unique_handle::operator ->() const noexcept
		{
			assert(handle);
			return handle.get();
		}
		template <typename T> typename unique_handle<T>::handle_type *unique_handle<T>::unique_handle::get() noexcept
		{
			return handle.get();
		}
		template <typename T> const typename unique_handle<T>::handle_type *unique_handle<T>::unique_handle::get() const noexcept
		{
			return handle.get();
		}
		template <typename T> const typename unique_handle<T>::deleter_type &unique_handle<T>::get_deleter() const noexcept
		{
			return deleter;
		}
		template <typename T> unique_handle<T>::operator bool() const noexcept
		{
			return handle;
		}

		// modifiers
		template <typename T> typename unique_handle<T>::handle_type unique_handle<T>::release() noexcept
		{
			handle_type handle = *this->handle;
			this->handle.reset();
			return handle;
		}
		template <typename T> void unique_handle<T>::reset()
		{
			unique_handle().swap(*this);
		}
		template <typename T> void unique_handle<T>::reset(handle_type handle)
		{
			unique_handle(handle).swap(*this);
		}
		template <typename T> void unique_handle<T>::reset(handle_type handle, deleter_type deleter)
		{
			unique_handle(handle, deleter).swap(*this);
		}
		template <typename T> void unique_handle<T>::reset(std::nullptr_t)
		{
			unique_handle(nullptr).swap(*this);
		}
		template <typename T> void unique_handle<T>::reset(std::nullptr_t, deleter_type deleter)
		{
			unique_handle(nullptr, deleter).swap(*this);
		}
		template <typename T> void unique_handle<T>::swap(unique_handle &other) noexcept
		{
			using util::swap;
			swap(handle,  other.handle);
			swap(deleter, other.deleter);
		}

		// factory functions
		template <typename T> unique_handle<T> make_unique_handle(T handle)
		{
			return unique_handle<T>(handle);
		}
		template <typename T> unique_handle<T> make_unique_handle(T handle, typename unique_handle<T>::deleter_type deleter)
		{
			return unique_handle<T>(handle, deleter);
		}
		
		// specialized algorithms
		template <typename T> void swap(unique_handle<T> &a, unique_handle<T> &b) noexcept
		{
			a.swap(b);
		}
	}
}
