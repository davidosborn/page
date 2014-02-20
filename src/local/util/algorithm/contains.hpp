/**
 * @copyright
 *
 * Copyright (c) 2006-2014 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the same place
 *    and form as other copyright, license, and disclaimer information.
 *
 * 3. Redistributions in binary form must also include an acknowledgement in the
 *    same place and form as other acknowledgements (such as the credits),
 *    similar in substance to the following:
 *
 *       Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#ifndef    page_local_util_algorithm_contains_hpp
#   define page_local_util_algorithm_contains_hpp

#	include <utility> // declval

#	include "../type_traits/sfinae.hpp" // DEFINE_SFINAE_TYPE_TRAIT, ENABLE_IF

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
					ENABLE_IF(!detail::has_find<Container>::value))
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
					ENABLE_IF(detail::has_find<Container>::value))
		{
			return container.find(value) != container.end();
		}
		///@}
	}
}

#endif
