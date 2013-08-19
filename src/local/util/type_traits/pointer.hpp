#ifndef    page_local_util_type_traits_pointer_hpp
#   define page_local_util_type_traits_pointer_hpp

#	include <type_traits>

namespace page
{
	namespace util
	{
		/**
		 * Removes one level of indirection from a type.
		 */
		template <typename T> struct remove_indirection
		{
			typedef decltype(*std::declval<T>())                    reference;
			typedef typename std::remove_reference<reference>::type type;
			typedef typename std::remove_cv<type>::type             value_type;
		};

		/**
		 * @defgroup remove_member_pointer
		 * @{
		 */
		namespace detail
		{
			template <typename T>
				struct remove_member_pointer_impl
			{
				typedef T type;
			};
			template <typename T, typename U>
				struct remove_member_pointer_impl<T U::*>
			{
				typedef T type;
			};
		}

		/**
		 * Removes the member pointer from a type, leaving the type of the member
		 * pointed to.
		 */
		template <typename T>
			struct remove_member_pointer :
				detail::remove_member_pointer_impl<T> {};
		///@}
	}
}

#endif
