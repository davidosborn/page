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

#include "../../../err/exception/throw.hpp" // THROW
#include "ClockTimer.hpp"

namespace page
{
	namespace sys
	{
		namespace posix
		{
			// update
			float ClockTimer::Step()
			{
				timespec newTime;
				if (clock_gettime(CLOCK_REALTIME, &newTime) == -1)
					THROW err::PlatformException<err::PosixPlatformTag>("failed to get real-time clock time");
				float delta = newTime.tv_sec - time.tv_sec +
					(newTime.tv_nsec - time.tv_nsec) / 1000000000.f;
				time = newTime;
				return delta;
			}
		}

		// factory function
		Timer *MakeTimer()
		{
			return new posix::ClockTimer;
		}
	}
}
