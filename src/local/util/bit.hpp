#ifndef    page_local_util_bit_hpp
#   define page_local_util_bit_hpp

namespace page
{
	namespace util
	{
		template <typename T> unsigned Bits(T);
		template <typename T> unsigned LowestBit(T);
		template <typename T> unsigned HighestBit(T);

		void BitCopy(const void *src, unsigned srcOffset, void *dest, unsigned destOffset, unsigned n);
	}
}

#	include "bit.tpp"
#endif
