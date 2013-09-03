namespace page
{
	namespace util
	{
		// default deleter
		template <typename T> struct scoped_handle<T>::DefaultDeleter
		{
			void operator ()(typename scoped_handle<T>::handle_type handle_type) const {}
		};

		// constructors
		template <typename T> scoped_handle<T>::scoped_handle() :
			scoped_handle(nullptr) {}
		template <typename T> scoped_handle<T>::scoped_handle(handle_type handle) :
			scoped_handle(handle, DefaultDeleter()) {}
		template <typename T> scoped_handle<T>::scoped_handle(handle_type handle, deleter_type deleter) :
			handle(handle), deleter(deleter) {}
		template <typename T> scoped_handle<T>::scoped_handle(std::nullptr_t) :
			scoped_handle(nullptr, DefaultDeleter()) {}
		template <typename T> scoped_handle<T>::scoped_handle(std::nullptr_t, deleter_type deleter) :
			deleter(deleter) {}

		// destructor
		template <typename T> scoped_handle<T>::~scoped_handle()
		{
			if (handle && deleter) deleter(*handle);
		}

		// observers
		template <typename T> typename scoped_handle<T>::handle_type &scoped_handle<T>::scoped_handle::operator *() noexcept
		{
			assert(handle);
			return *handle;
		}
		template <typename T> const typename scoped_handle<T>::handle_type &scoped_handle<T>::scoped_handle::operator *() const noexcept
		{
			assert(handle);
			return *handle;
		}
		template <typename T> typename scoped_handle<T>::handle_type *scoped_handle<T>::scoped_handle::operator ->() noexcept
		{
			assert(handle);
			return handle.get();
		}
		template <typename T> const typename scoped_handle<T>::handle_type *scoped_handle<T>::scoped_handle::operator ->() const noexcept
		{
			assert(handle);
			return handle.get();
		}
		template <typename T> typename scoped_handle<T>::handle_type *scoped_handle<T>::scoped_handle::get() noexcept
		{
			return handle.get();
		}
		template <typename T> const typename scoped_handle<T>::handle_type *scoped_handle<T>::scoped_handle::get() const noexcept
		{
			return handle.get();
		}
		template <typename T> const typename scoped_handle<T>::deleter_type &scoped_handle<T>::get_deleter() const noexcept
		{
			return deleter;
		}
		template <typename T> scoped_handle<T>::operator bool() const noexcept
		{
			return handle;
		}

		// modifiers
		template <typename T> typename scoped_handle<T>::handle_type scoped_handle<T>::release() noexcept
		{
			handle_type handle = *this->handle;
			this->handle.reset();
			return handle;
		}
		template <typename T> void scoped_handle<T>::reset()
		{
			scoped_handle().swap(*this);
		}
		template <typename T> void scoped_handle<T>::reset(handle_type handle)
		{
			scoped_handle(handle).swap(*this);
		}
		template <typename T> void scoped_handle<T>::reset(handle_type handle, deleter_type deleter)
		{
			scoped_handle(handle, deleter).swap(*this);
		}
		template <typename T> void scoped_handle<T>::reset(std::nullptr_t)
		{
			scoped_handle(nullptr).swap(*this);
		}
		template <typename T> void scoped_handle<T>::reset(std::nullptr_t, deleter_type deleter)
		{
			scoped_handle(nullptr, deleter).swap(*this);
		}
		template <typename T> void scoped_handle<T>::swap(scoped_handle &other) noexcept
		{
			using util::swap;
			swap(handle,  other.handle);
			swap(deleter, other.deleter);
		}

		// factory functions
		template <typename T> scoped_handle<T> make_scoped_handle(T handle)
		{
			return scoped_handle<T>(handle);
		}
		template <typename T> scoped_handle<T> make_scoped_handle(T handle, typename scoped_handle<T>::deleter_type deleter)
		{
			return scoped_handle<T>(handle, deleter);
		}

		// standard-library compatibility
		template <typename T> void swap(scoped_handle<T> &a, scoped_handle<T> &b) noexcept
		{
			a.swap(b);
		}
	}
}
