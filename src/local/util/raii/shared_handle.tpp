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
