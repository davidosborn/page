#ifndef    page_local_util_functional_pointer_hpp
#   define page_local_util_functional_pointer_hpp

#	include <memory> // addressof

#	include "../type_traits/pointer.hpp" // remove_indirection

namespace page
{
	namespace util
	{
		/**
		 * @defgroup functional-pointer
		 *
		 * Function objects for manipulating pointers.
		 *
		 * @{
		 */
		/**
		 * A function object for dereferencing pointers or pointer-like objects.
		 */
		template <typename T>
			struct dereference
		{
			typename util::remove_indirection<T>::reference
				operator ()(const T &a) const
			{
				return *a;
			}
		};

		/**
		 * A function object for applying the address-of operator.
		 *
		 * @note Not to be confused with @c std::addressof, which finds the real
		 *       address of an object, regardless of any overloading of the
		 *       address-of operator.
		 */
		template <typename T>
			struct address_of
		{
			T *operator ()(T &a) const
			{
				return &a;
			}
		};

		/**
		 * A function-object wrapper around @c std::addressof.
		 */
		template <typename T>
			struct real_address_of
		{
			T *operator ()(T &a) const
			{
				return std::addressof(a);
			}
		};
		///@}
	}
}

#endif
