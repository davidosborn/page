#include <cstdint> // uintptr_t

namespace page
{
	namespace util
	{
		// construct
		template <typename T, unsigned alignment> AlignedAllocator<T, alignment>::AlignedAllocator() throw() :
			std::allocator<T>() {}
		template <typename T, unsigned alignment> template <typename U, unsigned alignment2> AlignedAllocator<T, alignment>::AlignedAllocator(const AlignedAllocator<U, alignment2> &other) throw() :
			std::allocator<T>(other) {}

		// allocation
		template <typename T, unsigned alignment> typename AlignedAllocator<T, alignment>::pointer AlignedAllocator<T, alignment>::allocate(size_type n, typename std::allocator<void>::const_pointer hint)
		{
			n = sizeof(unsigned) + alignment - 1 + n * sizeof(pointer);
			pointer p = std::allocator<T>::allocate(n, hint);
			unsigned *offsetp = reinterpret_cast<unsigned *>(p);
			unsigned offset = (alignment - std::uintptr_t(offsetp + 1) % alignment) % alignment;
			*(offsetp = reinterpret_cast<unsigned *>(reinterpret_cast<char *>(offsetp) + offset)) = offset;
			return reinterpret_cast<pointer>(offsetp + 1);
		}
		template <typename T, unsigned alignment> void AlignedAllocator<T, alignment>::deallocate(pointer p, size_type n)
		{
			unsigned *offsetp = reinterpret_cast<unsigned *>(p) - 1;
			p = reinterpret_cast<pointer>(reinterpret_cast<char *>(offsetp) - *offsetp);
			std::allocator<T>::deallocate(p, n);
		}
	}
}
