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
