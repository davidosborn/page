#ifndef    page_local_util_functional_factory_hpp
#   define page_local_util_functional_factory_hpp

#	include <utility> // forward

namespace page
{
	namespace util
	{
		/**
		 * @defgroup functional-factory
		 *
		 * Function objects for operators that are missing from the C++
		 * Standard Library.
		 *
		 * @{
		 */
		/**
		 * A function object that wraps @c operator @c new.
		 */
		template <typename T, typename... Args>
			struct new_function
		{
			typedef T *result_type;
			typedef result_type signature(Args...);

			T *operator ()(Args &&... args) const
			{
				return new T(std::forward<Args>(args)...);
			}
		};

		/**
		 * A function object that wraps @c operator @c new and returns a
		 * @c std::unique_ptr.
		 */
		template <typename T, typename... Args>
			struct factory_function
		{
			typedef std::unique_ptr<T> result_type;
			typedef result_type signature(Args...);

			std::unique_ptr<T> operator ()(Args &&... args) const
			{
				return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
			}
		};

		/**
		 * A function object that wraps @c std::make_shared.
		 */
		template <typename T, typename... Args>
			struct make_shared_function
		{
			typedef std::shared_ptr<T> result_type;
			typedef result_type signature(Args...);

			std::shared_ptr<T> operator ()(Args &&... args) const
			{
				return std::make_shared<T>(std::forward<Args>(args)...);
			}
		};

		/**
		 * A function object that creates an object on the stack.
		 */
		template <typename T, typename... Args>
			struct constructor_function
		{
			typedef T result_type;
			typedef result_type signature(Args...);

			T operator ()(Args &&... args) const
			{
				return T(std::forward<Args>(args)...);
			}
		};
		///@}
	}
}

#endif
