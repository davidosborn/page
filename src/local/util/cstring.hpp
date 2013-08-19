#ifndef    page_local_util_cstring_hpp
#   define page_local_util_cstring_hpp

#	include <cstddef> // size_t

namespace page
{
	namespace util
	{
		void *memnot (void *dest, const void *src, size_t); // dest = ~src
		void *memxor (void *dest, const void *src, size_t); // dest ^= src
	}
}

#endif
