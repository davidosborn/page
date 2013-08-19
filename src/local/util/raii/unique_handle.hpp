#ifndef    page_local_util_raii_unique_handle_hpp
#   define page_local_util_raii_unique_handle_hpp

#	include <cstddef> // nullptr_t
#	include <functional> // function

#	include <boost/optional.hpp>

#	include "../class/special_member_functions.hpp" // Uncopyable

namespace page
{
	namespace util
	{
		/**
		 * Unique handle wrapper with custom deleter.
		 *
		 * @note This class supports incomplete types.
		 * @note The deleter may be initialized to @c nullptr on construction,
		 *       in which case it will not be executed on destruction.
		 *
		 * @ingroup smart-pointer
		 */
		template <typename T> struct unique_handle : Uncopyable<unique_handle<T>>
		{
			typedef T handle_type;
			typedef std::function<void (handle_type)> deleter_type;

			// constructors
			unique_handle();
			explicit unique_handle(handle_type);
			unique_handle(handle_type, deleter_type);
			explicit unique_handle(std::nullptr_t);
			unique_handle(std::nullptr_t, deleter_type);
			unique_handle(unique_handle &&);
			template <typename U> unique_handle(unique_handle<U> &&);

			// assignment
			unique_handle &operator =(unique_handle &&);
			template <typename U> unique_handle &operator =(unique_handle<U> &&);

			// destructor
			~unique_handle();

			// observers
			handle_type &operator  *() noexcept; const handle_type &operator  *() const noexcept;
			handle_type *operator ->() noexcept; const handle_type *operator ->() const noexcept;
			handle_type *get()         noexcept; const handle_type *get()         const noexcept;
			const deleter_type &get_deleter() const noexcept;
			explicit operator bool() const noexcept;

			// modifiers
			handle_type release() noexcept;
			void reset();
			void reset(handle_type);
			void reset(handle_type, deleter_type);
			void reset(std::nullptr_t);
			void reset(std::nullptr_t, deleter_type);
			void swap(unique_handle &) noexcept;

			private:
			class DefaultDeleter;

			boost::optional<handle_type> handle;
			deleter_type deleter;
		};

		// factory functions
		template <typename T> unique_handle<T> make_unique_handle(T);
		template <typename T> unique_handle<T> make_unique_handle(T, typename unique_handle<T>::deleter_type);

		// specialized algorithms
		template <typename T> void swap(unique_handle<T> &, unique_handle<T> &) noexcept;
	}
}

#	include "unique_handle.tpp"
#endif
