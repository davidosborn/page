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

#include <algorithm> // max
#include "../log/stats.hpp" // GetStats, Stats::IncFrame
#include "../math/float.hpp" // Inf
#include "Timer.hpp"

namespace page
{
	namespace sys
	{
		// construct/destroy
		Timer::Timer() : delta(0), elapsed(0), queue(0), paused(false) {}
		Timer::~Timer() {}

		// state
		float Timer::GetDelta() const
		{
			return delta;
		}
		float Timer::GetElapsed() const
		{
			return elapsed;
		}
		float Timer::GetFrameRate() const
		{
			return delta ? 1 / delta : math::Inf();
		}

		// update
		void Timer::Update()
		{
			delta = queue;
			if (!paused) delta += Step();
			elapsed += delta;
			queue = 0;
			log::GetStats().IncFrame(delta);
		}

		// modifiers
		void Timer::Pause()
		{
			if (!paused)
			{
				queue = Step();
				paused = true;
			}
		}
		void Timer::Resume()
		{
			if (paused)
			{
				queue = std::max(queue - Step(), 0.f);
				paused = false;
			}
		}
		void Timer::Reset()
		{
			delta = elapsed = 0;
		}
	}
}
