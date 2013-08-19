#ifndef    page_local_util_memory_Share_hpp
#   define page_local_util_memory_Share_hpp

#	include <memory> // shared_ptr

#	include "NopDeleter.hpp"

namespace page { namespace util
{
	/**
	 * Returns a @c std::shared_ptr representation of an object by
	 * reference.
	 *
	 * @note The @c std::shared_ptr does not take ownership of the memory.
	 */
	template <typename T>
		std::shared_ptr<T> Share(T &p)
	{
		return std::shared_ptr<T>(&p, NopDeleter<T>());
	}
}}

#endif
