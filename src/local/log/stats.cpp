#include "Stats.hpp"

namespace page
{
	namespace log
	{
		/*--------------------------+
		| constructors & destructor |
		+--------------------------*/

		Stats::Stats() :
			runTime(0), frameCount(0),
			cacheTries(0), cacheMisses(0), frameRate(0) {}

		/*----------+
		| observers |
		+----------*/

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

		/*----------+
		| modifiers |
		+----------*/

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
	}
}
