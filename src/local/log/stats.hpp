#ifndef    page_local_log_Stats_hpp
#   define page_local_log_Stats_hpp

#	include "../util/class/Monostate.hpp"

namespace page
{
	namespace log
	{
		class Stats :
			public util::Monostate<Stats>
		{
			/*-------------+
			| constructors |
			+-------------*/

			public:
			Stats();

			/*----------+
			| observers |
			+----------*/

			public:
			unsigned GetRunTime() const;
			unsigned GetFrameCount() const;
			float GetFrameRate() const;
			unsigned GetCacheTries() const;
			unsigned GetCacheMisses() const;
			float GetCacheCoherence() const;

			/*----------+
			| modifiers |
			+----------*/

			public:
			void IncFrame(float deltaTime);
			void IncCacheTries();
			void IncCacheMisses();
			void Reset();

			/*-------------+
			| data members |
			+-------------*/

			private:
			unsigned runTime     = 0;
			unsigned frameCount  = 0;
			unsigned cacheTries  = 0;
			unsigned cacheMisses = 0;
			float    frameRate   = 0;
		};
	}
}

#endif
