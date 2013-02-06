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

#include "stats.hpp"

namespace page
{
	namespace log
	{
		// construct
		Stats::Stats() : runTime(0), frameCount(0),
			cacheTries(0), cacheMisses(0), frameRate(0) {}

		// state
		unsigned Stats::GetRunTime() const
		{
			return runTime;
		}
		unsigned Stats::GetFrameCount() const
		{
			return frameCount;
		}
		float Stats::GetFrameRate() const
		{
			return frameRate;
		}
		unsigned Stats::GetCacheTries() const
		{
			return cacheTries;
		}
		unsigned Stats::GetCacheMisses() const
		{
			return cacheMisses;
		}
		float Stats::GetCacheCoherence() const
		{
			return float(cacheTries - cacheMisses) / cacheTries;
		}

		// modifiers
		void Stats::IncFrame(float deltaTime)
		{
			runTime += deltaTime;
			++frameCount;
			frameRate = 1 / deltaTime;
		}
		void Stats::IncCacheTries()
		{
			++cacheTries;
		}
		void Stats::IncCacheMisses()
		{
			++cacheMisses;
		}
		void Stats::Reset()
		{
			runTime = frameCount = cacheTries = cacheMisses = 0;
			frameRate = 0;
		}

		// global access
		Stats &GetStats()
		{
			static Stats stats;
			return stats;
		}
	}
}
