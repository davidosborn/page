#ifndef    page_local_util_type_traits_const_volatile_hpp
#   define page_local_util_type_traits_const_volatile_hpp

#	include <type_traits>

namespace page { namespace util
{
	/**
	 * @defgroup copy_const
	 * @{
	 */
	namespace detail
	{
		template <typename To, typename From, bool = std::is_const<From>::value>
			struct copy_const_impl
		{
			typedef typename std::add_const<To>::type type;
		};
		template <typename To, typename From>
			struct copy_const_impl<To, From, false>
		{
			typedef typename std::remove_const<To>::type type;
		};
	}

	/**
	 * Copies const qualifiers from one type to another.
	 */
	template <typename To, typename From>
		struct copy_const
	{
		typedef typename detail::copy_const_impl<To, From>::type type;
	};
	///@}

	/**
	 * @defgroup copy_volatile
	 * @{
	 */
	namespace detail
	{
		template <typename To, typename From, bool = std::is_volatile<From>::value>
			struct copy_volatile_impl
		{
			typedef typename std::add_volatile<To>::type type;
		};
		template <typename To, typename From>
			struct copy_volatile_impl<To, From, false>
		{
			typedef typename std::remove_volatile<To>::type type;
		};
	}

	/**
	 * Copies volatile qualifiers from one type to another.
	 */
	template <typename To, typename From>
		struct copy_volatile
	{
		typedef typename detail::copy_volatile_impl<To, From>::type type;
	};
	///@}

	/**
	 * @defgroup copy_cv
	 * @{
	 */
	/**
	 * Copies const/volatile qualifiers from one type to another.
	 */
	template <typename To, typename From>
		struct copy_cv
	{
		typedef
			typename copy_const<
			typename copy_volatile<To, From>::type, From>::type type;
	};
	///@}
}}

#endif
