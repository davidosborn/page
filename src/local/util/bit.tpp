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

namespace page
{
	namespace util
	{
		template <typename T> unsigned Bits(T n)
		{
			unsigned r = 0;
			while (n &= n - 1) r++;
			return r;
		}
		template <typename T> unsigned LowestBit(T n)
		{
			unsigned r = 0;
			while (!(n & 1 << r)) if (++r == sizeof n) return 0;
			return r;
		}
		template <typename T> unsigned HighestBit(T n)
		{
			unsigned r = sizeof n - 1;
			while (!(n & 1 << r)) if (!r--) return 0;
			return r;
		}
	}
}
