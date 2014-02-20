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

#ifndef    page_local_inp_PollState_hpp
#   define page_local_inp_PollState_hpp

#	include <vector>

#	include "../math/Euler.hpp"
#	include "../math/Vector.hpp"
#	include "Key.hpp"
#	include "Modifier.hpp"

namespace page { namespace inp
{
	/**
	 * The cursor state for Driver::CursorMode::look.
	 */
	struct CursorLookState
	{
		math::Euler<> rotation;
		float lift = false;
		float zoom = false;
	};

	/**
	 * The control state.
	 */
	struct ControlState
	{
		math::Vec2 direction;
		Modifier::Type modifiers = Modifier::none;
	};

	/**
	 * The frame-by-frame state as reported by the driver and its devices when
	 * they are polled.
	 */
	struct PollState
	{
		/**
		 * The cursor state for CursorMode::look.
		 */
		CursorLookState look;

		/**
		 * The control state.
		 */
		ControlState control;

		/**
		 * The sequence of keys that have been pressed since the last frame.
		 */
		std::vector<Key> keyEvents;

		/**
		 * The sequence of characters that have been entered since the last
		 * frame.
		 */
		std::vector<char> charEvents;
	};

	/*--------------+
	| concatenation |
	+--------------*/

	CursorLookState  operator + (CursorLookState,   const CursorLookState &);
	CursorLookState &operator +=(CursorLookState &, const CursorLookState &);

	ControlState  operator + (ControlState,   const ControlState &);
	ControlState &operator +=(ControlState &, const ControlState &);

	PollState  operator + (PollState,   const PollState &);
	PollState &operator +=(PollState &, const PollState &);
}}

#endif
