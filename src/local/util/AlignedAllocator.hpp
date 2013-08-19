#ifndef    page_local_util_AlignedAllocator_hpp
#   define page_local_util_AlignedAllocator_hpp

#	include <memory> // allocator

namespace page
{
	namespace util
	{
		template <typename T, unsigned alignment> struct AlignedAllocator :
			std::allocator<T>
		{
			// HACK: GCC doesn't support using typename (bug #14258)
			typedef typename std::allocator<T>::size_type size_type;
			typedef typename std::allocator<T>::pointer pointer;
			typedef typename std::allocator<T>::const_pointer const_pointer;

			// construct
			AlignedAllocator() throw();
			template <typename U, unsigned alignment2> AlignedAllocator(const AlignedAllocator<U, alignment2> &) throw();
			template <typename U> struct rebind { typedef AlignedAllocator<U, alignment> other; };

			// allocation
			pointer allocate(size_type, typename std::allocator<void>::const_pointer hint = 0);
			void deallocate(pointer, size_type);
		};
	}
}

#	include "AlignedAllocator.tpp"
#endif
