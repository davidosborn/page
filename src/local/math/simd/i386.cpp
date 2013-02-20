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

#include "../../sys/i386.hpp"
#include "i386/3dnow.hpp"
#include "i386/3dnowext.hpp"
#include "i386/mmx.hpp"
#include "i386/sse.hpp"
#include "i386/sse2.hpp"
#include "i386/sse3.hpp"

namespace page
{
	namespace math
	{
		namespace simd
		{
			using namespace i386;
			using namespace sys::i386;

			void Init()
			{
				if (CheckMmx())
				{
					MulMatrix2f = mmx::MulMatrix2f;
					MulMatrix3f = mmx::MulMatrix3f;
					MulMatrix34f = mmx::MulMatrix34f;
					MulMatrix4f = mmx::MulMatrix4f;
				}
				if (CheckSse())
				{
					MulMatrix2f = sse::MulMatrix2f;
					MulMatrix3f = sse::MulMatrix3f;
					MulMatrix34f = sse::MulMatrix34f;
					MulMatrix4f = sse::MulMatrix4f;
				}
				if (CheckSse2())
				{
					MulMatrix2f = sse2::MulMatrix2f;
					MulMatrix3f = sse2::MulMatrix3f;
					MulMatrix34f = sse2::MulMatrix34f;
					MulMatrix4f = sse2::MulMatrix4f;
				}
				if (CheckSse3())
				{
					MulMatrix2f = sse3::MulMatrix2f;
					MulMatrix3f = sse3::MulMatrix3f;
					MulMatrix34f = sse3::MulMatrix34f;
					MulMatrix4f = sse3::MulMatrix4f;
				}
				if (Check3dnow())
				{
					MulMatrix2f = 3dnow::MulMatrix2f;
					MulMatrix3f = 3dnow::MulMatrix3f;
					MulMatrix34f = 3dnow::MulMatrix34f;
					MulMatrix4f = 3dnow::MulMatrix4f;
				}
				if (Check3dnowext())
				{
					MulMatrix2f = 3dnowext::MulMatrix2f;
					MulMatrix3f = 3dnowext::MulMatrix3f;
					MulMatrix34f = 3dnowext::MulMatrix34f;
					MulMatrix4f = 3dnowext::MulMatrix4f;
				}
			}
		}
	}
}
