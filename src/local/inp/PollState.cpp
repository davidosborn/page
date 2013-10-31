#include "PollState.hpp"

namespace page { namespace inp
{
	/*--------------+
	| concatenation |
	+--------------*/

	CursorLookState operator +(CursorLookState a, const CursorLookState &b)
	{
		a += b;
		return a;
	}

	CursorLookState &operator +=(CursorLookState &a, const CursorLookState &b)
	{
		a.rotation += b.rotation;
		a.lift     += b.lift;
		a.zoom     += b.zoom;
		return a;
	}

	ControlState operator +(ControlState a, const ControlState &b)
	{
		a += b;
		return a;
	}

	ControlState &operator +=(ControlState &a, const ControlState &b)
	{
		a.direction = Min(Max(a.direction + b.direction, -1), 1);
		a.modifiers |= b.modifiers;
		return a;
	}

	PollState operator +(PollState a, const PollState &b)
	{
		a += b;
		return a;
	}

	PollState &operator +=(PollState &a, const PollState &b)
	{
		a.control += b.control;
		a.look    += b.look;

		a.keyEvents.insert(a.keyEvents.end(), b.keyEvents.begin(), b.keyEvents.end());
		a.charEvents.insert(a.charEvents.end(), b.charEvents.begin(), b.charEvents.end());

		return a;
	}
}}
