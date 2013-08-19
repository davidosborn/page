#ifndef    page_local_inp_State_hpp
#   define page_local_inp_State_hpp

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

				math::Vec2 direction;
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
