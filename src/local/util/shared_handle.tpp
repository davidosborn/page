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
		template <typename T> struct shared_handle<T>::DefaultDeleter
		{
			void operator ()(typename shared_handle<T>::handle_type handle_type) const {}
		};
		
		// deleter adapter
		template <typename T> struct shared_handle<T>::DeleterAdapter
		{
			void operator ()(const handle_type *handle, deleter_type deleter) const
			{
				deleter(*handle);
				delete handle;
			}
		};
		
		// constructors
		template <typename T> shared_handle<T>::shared_handle() {}
		template <typename T> shared_handle<T>::shared_handle(handle_type handle) :
			shared_handle(handle, DefaultDeleter()) {}
		template <typename T> shared_handle<T>::shared_handle(handle_type handle, deleter_type deleter) :
			deleter(deleter)
		{
			try
			{
				this->handle.reset(new T(handle),
					std::bind(DeleterAdapter(), std::placeholders::_1, deleter));
			}
			catch (...)
			{
				deleter(handle);
				throw;
			}
		}
		template <typename T> shared_handle<T>::shared_handle(std::nullptr_t) :
			shared_handle(handle, DefaultDeleter()) {}
		template <typename T> shared_handle<T>::shared_handle(std::nullptr_t, deleter_type deleter) :
			handle(nullptr, std::bind(DeleterAdapter(), std::placeholders::_1, deleter)),
			deleter(deleter) {}

		// observers
		template <typename T> typename shared_handle<T>::handle_type &shared_handle<T>::shared_handle::operator *() noexcept
		{
			assert(handle);
			return *handle;
		}
		template <typename T> const typename shared_handle<T>::handle_type &shared_handle<T>::shared_handle::operator *() const noexcept
		{
			assert(handle);
			return *handle;
		}
		template <typename T> typename shared_handle<T>::handle_type *shared_handle<T>::shared_handle::operator ->() noexcept
		{
			assert(handle);
			return handle.get();
		}
		template <typename T> const typename shared_handle<T>::handle_type *shared_handle<T>::shared_handle::operator ->() const noexcept
		{
			assert(handle);
			return handle.get();
		}
		template <typename T> typename shared_handle<T>::handle_type *shared_handle<T>::shared_handle::get() noexcept
		{
			return handle.get();
		}
		template <typename T> const typename shared_handle<T>::handle_type *shared_handle<T>::shared_handle::get() const noexcept
		{
			return handle.get();
		}
		template <typename T> const typename shared_handle<T>::deleter_type &shared_handle<T>::get_deleter() const noexcept
		{
			return deleter;
		}
		template <typename T> shared_handle<T>::operator bool() const noexcept
		{
			return handle;
		}

		// modifiers
		template <typename T> void shared_handle<T>::reset()
		{
			shared_handle().swap(*this);
		}
		template <typename T> void shared_handle<T>::reset(handle_type handle)
		{
			shared_handle(handle).swap(*this);
		}
		template <typename T> void shared_handle<T>::reset(handle_type handle, deleter_type deleter)
		{
			shared_handle(handle, deleter).swap(*this);
		}
		template <typename T> void shared_handle<T>::reset(std::nullptr_t)
		{
			shared_handle(nullptr).swap(*this);
		}
		template <typename T> void shared_handle<T>::reset(std::nullptr_t, deleter_type deleter)
		{
			shared_handle(nullptr, deleter).swap(*this);
		}
		template <typename T> void shared_handle<T>::swap(shared_handle &other) noexcept
		{
			using std::swap;
			swap(handle,  other.handle);
			swap(deleter, other.deleter);
		}

		// factory functions
		template <typename T> shared_handle<T> make_shared_handle(T handle)
		{
			return shared_handle<T>(handle);
		}
		template <typename T> shared_handle<T> make_shared_handle(T handle, typename shared_handle<T>::deleter_type deleter)
		{
			return shared_handle<T>(handle, deleter);
		}
		
		// specialized algorithms
		template <typename T> void swap(shared_handle<T> &a, shared_handle<T> &b) noexcept
		{
			a.swap(b);
		}
	}
}
