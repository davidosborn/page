#include "State.hpp"

namespace page
{
	namespace inp
	{
		// control construct
		State::Control::Control() : modifiers() {}

		// look construct
		State::Look::Look() : lift(0), zoom(0) {}

		// concatenation
		State operator +(const State &a, const State &b)
		{
			State r =
			{
				a.control + b.control,
				a.look    + b.look,
				// events
				a.keyEvents,
				a.charEvents
			};
			r.keyEvents.insert(r.keyEvents.end(), b.keyEvents.begin(), b.keyEvents.end());
			r.charEvents.insert(r.charEvents.end(), b.charEvents.begin(), b.charEvents.end());
			return r;
		}
		State &operator +=(State &a, const State &b)
		{
			return a = a + b;
		}
		State::Control operator +(const State::Control &a, const State::Control &b)
		{
			State::Control r;
			r.direction = Min(Max(a.direction + b.direction, -1), 1);
			for (State::Control::Modifiers::size_type i = 0; i < r.modifiers.size(); ++i)
				r.modifiers[i] = a.modifiers[i] || b.modifiers[i];
			return r;
		}
		State::Control &operator +=(State::Control &a, const State::Control &b)
		{
			return a = a + b;
		}
		State::Look operator +(const State::Look &a, const State::Look &b)
		{
			State::Look r;
			r.rotation = a.rotation + b.rotation;
			r.lift     = a.lift     + b.lift;
			r.zoom     = a.zoom     + b.zoom;
			return r;
		}
		State::Look &operator +=(State::Look &a, const State::Look &b)
		{
			return a = a + b;
		}
	}
}
