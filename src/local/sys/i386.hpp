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

#ifndef    page_local_sys_i386_hpp
#   define page_local_sys_i386_hpp

#	include <array>
#	include <cstdint>

namespace page
{
	namespace sys
	{
		namespace i386
		{
			namespace detail
			{
				bool CheckCpuid();
				std::array<std::uint32, 4> Cpuid(unsigned function);
			}

			inline bool CheckMmx()
			{
				static bool have = detail::Cpuid(1)[3] & 0x800000;
				return have;
			}
			inline bool CheckSse()
			{
				static bool have = detail::Cpuid(1)[3] & 0x2000000;
				return have;
			}
			inline bool CheckSse2()
			{
				static bool have = detail::Cpuid(1)[3] & 0x4000000;
				return have;
			}
			inline bool CheckSse3()
			{
				static bool have = detail::Cpuid(1)[2] & 1;
				return have;
			}
			inline bool Check3dnow()
			{
				static bool have = detail::Cpuid(1)[3] & 0x80000000;
				return have;
			}
			inline bool Check3dnowext()
			{
				static bool have = detail::Cpuid(1)[3] & 0x40000000;
				return have;
			}
		}
	}
}

#endif
