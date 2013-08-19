#include <climits> // CHAR_BIT, UCHAR_MAX

namespace page
{
	namespace util
	{
		void BitCopy(const void *srcBase, unsigned srcOffset, void *destBase, unsigned destOffset, unsigned n)
		{
			const unsigned char *src = static_cast<const unsigned char *>(srcBase);
			unsigned char *dest = static_cast<unsigned char *>(destBase);
			// normalize offsets
			src  += srcOffset  / CHAR_BIT; srcOffset  %= CHAR_BIT;
			dest += destOffset / CHAR_BIT; destOffset %= CHAR_BIT;
			// align destination
			if (destOffset)
			{
				unsigned align = CHAR_BIT - destOffset;
				if (n > align)
				{
					if (srcOffset > destOffset)
					{
						*dest = *dest & UCHAR_MAX << align | *src++ << srcOffset >> destOffset;
						*dest |= *src >> CHAR_BIT - (srcOffset - destOffset);
						srcOffset -= destOffset;
					}
					else
					{
						*dest = *dest & UCHAR_MAX << align | *src << srcOffset >> destOffset;
						if ((srcOffset += align) == CHAR_BIT)
						{
							++src;
							srcOffset = 0;
						}
					}
					++dest;
					destOffset = 0;
					n -= align;
				}
				else
				{
					if (n > CHAR_BIT - srcOffset)
					{
						*dest = *dest & (UCHAR_MAX << align | UCHAR_MAX >> destOffset + n) | *src++ << srcOffset >> destOffset;
						*dest |= *src >> destOffset + (CHAR_BIT - srcOffset) & UCHAR_MAX << CHAR_BIT - (destOffset + n);
					}
					else
					{
						*dest = *dest & (UCHAR_MAX << align | UCHAR_MAX >> destOffset + n) | *src >> CHAR_BIT - (srcOffset + n) << CHAR_BIT - n >> destOffset;
					}
					return;
				}
			}
			// copy full bytes
			if (n > CHAR_BIT)
			{
				if (srcOffset)
				{
					do
					{
						*dest = *src++ << srcOffset;
						*dest++ |= *src >> CHAR_BIT - srcOffset;
					}
					while ((n -= CHAR_BIT) > CHAR_BIT);
				}
				else
				{
					do
					{
						*dest++ = *src++;
					}
					while ((n -= CHAR_BIT) > CHAR_BIT);
				}
			}
			// copy remaining bits
			if (n > CHAR_BIT - srcOffset)
			{
				*dest = *dest & UCHAR_MAX >> n | *src++ << srcOffset;
				*dest |= *src >> CHAR_BIT - srcOffset & UCHAR_MAX << CHAR_BIT - n;
			}
			else
			{
				*dest = *dest & UCHAR_MAX >> n | (*src << srcOffset & UCHAR_MAX << CHAR_BIT - n);
			}
		}
	}
}
