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

#include "../../../err/Exception.hpp"
#include "PcTimer.hpp"

namespace page
{
	namespace sys
	{
		namespace win32
		{
			// construct
			PcTimer::PcTimer()
			{
				if (!QueryPerformanceFrequency(&freq))
					THROW((err::Exception<err::SysModuleTag, err::Win32PlatformTag>("failed to initialize performance counter")))
				if (!QueryPerformanceCounter(&time))
					THROW((err::Exception<err::SysModuleTag, err::Win32PlatformTag>("failed to query performance counter")))
			}

			// update
			float PcTimer::Step()
			{
				LARGE_INTEGER newTime;
				if (!QueryPerformanceCounter(&newTime))
					THROW((err::Exception<err::SysModuleTag, err::Win32PlatformTag>("failed to query performance counter")))
				float delta = static_cast<long double>(newTime.QuadPart - time.QuadPart) / freq.QuadPart;
				time = newTime;
				return delta;
			}
		}

		// factory function
		Timer *MakeTimer()
		{
			return new win32::PcTimer;
		}
	}
}
