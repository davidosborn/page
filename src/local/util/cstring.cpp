#include <cstddef> // size_t

namespace page
{
	namespace util
	{
		void *memnot(void *dest, const void *src, size_t n)
		{
			for (size_t i = 0; i < n; ++i)
				static_cast<char *>(dest)[i] = ~
				static_cast<const char *>(src)[i];
			return dest;
		}
		void *memxor(void *dest, const void *src, size_t n)
		{
			for (size_t i = 0; i < n; ++i)
				static_cast<char *>(dest)[i] ^=
				static_cast<const char *>(src)[i];
			return dest;
		}
	}
}
