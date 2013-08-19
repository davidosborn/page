#include <algorithm> // max

#include "../log/Stats.hpp"
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
			GLOBAL(log::Stats).IncFrame(delta);
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
