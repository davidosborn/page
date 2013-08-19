#ifndef    page_local_util_type_traits_stream_hpp
#   define page_local_util_type_traits_stream_hpp

#	include <utility> // declval

#	include "sfinae.hpp"

namespace page
{
	namespace util
	{
		/**
		 * Detects whether a type can be inserted into another type, such as an
		 * output stream.
		 */
		DEFINE_SFINAE_TYPE_TRAIT_2(is_insertable,
			decltype(std::declval<U>() << std::declval<T>(), std::declval<void>()))

		/**
		 * Detects whether a type can be extracted from another type, such as an
		 * input stream.
		 */
		DEFINE_SFINAE_TYPE_TRAIT_2(is_extractable,
			decltype(std::declval<U>() >> std::declval<T>(), std::declval<void>()))
	}
}

#endif
