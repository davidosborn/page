/**
 * @section license
 *
 * Copyright (c) 2006-2013 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution, and in the same
 *    place and form as other copyright, license, and disclaimer information.
 *
 * As a special exception, distributions of derivative works in binary form may
 * include an acknowledgement in place of the above copyright notice, this list
 * of conditions, and the following disclaimer in the documentation and/or other
 * materials provided with the distribution, and in the same place and form as
 * other acknowledgements, similar in substance to the following:
 *
 *    Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

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
