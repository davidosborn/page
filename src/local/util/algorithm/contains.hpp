#ifndef    page_local_util_algorithm_contains_hpp
#   define page_local_util_algorithm_contains_hpp

#	include <utility> // declval

#	include "../type_traits/sfinae.hpp"

namespace page
{
	namespace util
	{
		/**
		 * @defgroup contains
		 * @{
		 */
		namespace detail
		{
			DEFINE_SFINAE_TYPE_TRAIT_1(has_find,
				decltype(
					std::declval<T>().find(
						std::declval<typename T::value_type>()),
					std::declval<void>()))
		}

		/**
		 * @return @c true if the specified container contains the specified
		 *         value.
		 */
		template <typename Container, typename T>
				bool contains(const Container &container, const T &value,
					ENABLE_IF((!detail::has_find<Container>::value)))
		{
			for (const auto &element : container)
				if (element == value) return true;
			return false;
		}

		/**
		 * @return @c true if the specified container contains the specified
		 *         value.
		 */
		template <typename Container, typename T>
				bool contains(const Container &container, const T &value,
					ENABLE_IF((detail::has_find<Container>::value)))
		{
			return container.find(value) != container.end();
		}
		///@}
	}
}

#endif
