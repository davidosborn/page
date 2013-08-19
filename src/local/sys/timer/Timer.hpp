#ifndef    page_local_sys_timer_Timer_hpp
#   define page_local_sys_timer_Timer_hpp

namespace page
{
	namespace sys
	{
		struct Timer
		{
			// construct/destroy
			Timer();
			virtual ~Timer();

			// state
			float GetDelta() const;
			float GetElapsed() const;
			float GetFrameRate() const;

			// update
			void Update();

			// modifiers
			void Pause();
			void Resume();
			void Reset();

			private:
			virtual float Step() = 0;

			float delta, elapsed, queue;
			bool paused;
		};

		// factory function
		Timer *MakeTimer();
	}
}

#endif
