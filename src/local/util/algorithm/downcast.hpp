#ifndef    page_local_util_algorithm_downcast_hpp
#   define page_local_util_algorithm_downcast_hpp

#	include <type_traits> // is_polymorphic

#	include "../type_traits/sfinae.hpp" // ENABLE_IF

namespace page { namespace util
{
	template <typename T, typename U>
		T downcast(U &&x, ENABLE_IF(std::is_polymorphic<U>::value))
	{
		return dynamic_cast<T>(std::forward<U>(x));
	}

	template <typename T, typename U>
		T downcast(U &&x, ENABLE_IF(!std::is_polymorphic<U>::value))
	{
		return static_cast<T>(std::forward<U>(x));
	}
}}

#endif
