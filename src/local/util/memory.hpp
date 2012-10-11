#ifndef    page_local_util_memory_hpp
#	define page_local_util_memory_hpp

#	include <memory> // shared_ptr

namespace page
{
	namespace util
	{
		/**
		 * A no-op deleter for use with @c shared_ptr and @c unique_ptr.
		 */
		struct NopDeleter
		{
			void operator ()(const void *) {}
		};
		
		/**
		 * Returns a @c std::shared_ptr representation of a static instance.
		 */
		template <typename T>
			std::shared_ptr<T> Share(T &p)
		{
			return std::shared_ptr<T>(&p, NopDeleter());
		}
	}
}

#endif
