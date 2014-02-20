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
