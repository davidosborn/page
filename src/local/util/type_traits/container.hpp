#ifndef    page_local_util_type_traits_container_hpp
#   define page_local_util_type_traits_container_hpp

#	include <type_traits>
#	include <utility> // declval

#	include "sfinae.hpp"

namespace page { namespace util
{
	/**
	 * Detects whether a container is compatible with @c std::insert_iterator.
	 *
	 * Compatibility depends on whether the expression @c a.insert(p,t) is
	 * valid, which is one of the requirements for sequence and associative
	 * containers (ISO/IEC N3242 23.2.3).
	 */
	DEFINE_SFINAE_TYPE_TRAIT_1(is_compatible_with_insert_iterator,
		decltype(
			std::declval<T>().insert(
				// HACK: GCC doesn't accept const_iterator
				std::declval<typename T::iterator>(),
				std::declval<typename T::value_type>()),
			std::declval<void>()))
}}

#endif
