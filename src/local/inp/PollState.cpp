/**
 * @copyright
 *
 * Copyright (c) 2006-2014 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the same place
 *    and form as other copyright, license, and disclaimer information.
 *
 * 3. Redistributions in binary form must also include an acknowledgement in the
 *    same place and form as other acknowledgements (such as the credits),
 *    similar in substance to the following:
 *
 *       Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

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
