#ifndef    page_local_util_raii_factory_ptr_hpp
#   define page_local_util_raii_factory_ptr_hpp

#	include <cstddef> // nullptr_t
#	include <functional> // function
#	include <memory> // pointer_traits

namespace page { namespace util
{
	/**
	 * A smart-pointer facade for nullary factory-functions.
	 */
	template <typename T>
		class factory_ptr
	{
		template <typename>
			friend class factory_ptr;

		/*-------+
		| traits |
		+-------*/

		public:
		/**
		 * The type of a pointer to the product, returned by the factory
		 * function.
		 */
		using pointer = T;

		/**
		 * The type of the product.
		 */
		using element_type = typename std::pointer_traits<T>::element_type;

		/**
		 * The type of the factory function.
		 */
		using factory_function = std::function<pointer()>;

		/*-------------+
		| constructors |
		+-------------*/

		factory_ptr() noexcept = default;

		factory_ptr(std::nullptr_t) noexcept;

		protected:
		explicit factory_ptr(const factory_function &);

		public:
		template <typename U>
			factory_ptr(const factory_ptr<U> &);

		/*----------+
		| observers |
		+----------*/

		/**
		 * @return @c *lock().
		 */
		element_type &operator *() const;

		/**
		 * @return @c lock().
		 */
		pointer operator ->() const;

		/**
		 * @return lock().
		 */
		pointer get() const;

		/**
		 * @return f().
		 */
		pointer lock() const;

		/**
		 * @return f.
		 */
		explicit operator bool() const noexcept;

		/*-------------+
		| data members |
		+-------------*/

		private:
		/**
		 * The factory function.
		 */
		factory_function f;
	};
}}

#	include "factory_ptr.tpp"
#endif
