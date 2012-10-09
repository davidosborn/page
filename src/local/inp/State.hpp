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

#ifndef    page_inp_State_hpp
#   define page_inp_State_hpp

#	include <array>
#	include <vector>
#	include "../math/Euler.hpp"
#	include "../math/Vector.hpp"
#	include "Key.hpp"

namespace page
{
	namespace inp
	{
		struct State
		{
			// control
			struct Control
			{
				// construct
				Control();

				math::Vector<2> direction;
				typedef std::array<bool, 2> Modifiers;
				Modifiers modifiers;
			} control;

			// look
			// NOTE: used during mouse-look mode
			struct Look
			{
				// construct
				Look();

				math::Euler<> rotation;
				float lift, zoom;
			} look;

			// events
			typedef std::vector<Key> KeyEvents;
			KeyEvents keyEvents;
			typedef std::vector<char> CharEvents;
			CharEvents charEvents;
		};

		// concatenation
		State operator +(const State &, const State &);
		State &operator +=(State &, const State &);
		State::Control operator +(const State::Control &, const State::Control &);
		State::Control &operator +=(State::Control &, const State::Control &);
		State::Look operator +(const State::Look &, const State::Look &);
		State::Look &operator +=(State::Look &, const State::Look &);
	}
}

#endif
