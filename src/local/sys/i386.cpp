/**
 * @section license
 *
 * Copyright (c) 2006-2012 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
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

#include <array>
#include <cstdint>

namespace page
{
	namespace sys
	{
		namespace detail
		{
			namespace
			{
				bool DoCheckCpuid()
				{
					bool result;
					__asm__ (
						// try to toggle CPUID bit in EFLAGS
						"pushf\n\t"
						"popl %%eax\n\t"
						"movl %%eax, %%edx\n\t"
						"xorl $0x200000, %%eax\n\t"
						"pushl %%eax\n\t"
						"popf\n\t"
						// check if bit was toggled
						"pushf\n\t"
						"popl %%eax\n\t"
						"xorl %%eax, %%edx\n\n"
						: "=a"(result) : : "%edx");
					return result;
				}
			}

			bool CheckCpuid()
			{
				static bool have = DoCheckCpuid();
				return have;
			}
			std::array<std::uint32_t, 4> Cpuid(unsigned fn)
			{
				std::array<std::uint32_t, 4> r = {};
				if (CheckCpuid())
				{
					std::uint32_t ecx, edx;
					__asm__ ("cpuid" : "=a"(r[0]), "=b"(r[1]), "=c"(r[2]), "=d"(r[3]) : "a"(fn));
				}
				return r;
			}
		}
	}
}
