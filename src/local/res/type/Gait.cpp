#include "Gait.hpp"
#include "Registry.hpp" // REGISTER_TYPE

namespace page
{
	namespace res
	{
		void PostLoadGait(Gait &gait)
		{
			if (!gait.run.animation)   gait.run   = gait.walk;
			if (!gait.sneak.animation) gait.sneak = gait.walk;
		}

		REGISTER_TYPE(Gait, "gait", PostLoadGait)
	}
}
