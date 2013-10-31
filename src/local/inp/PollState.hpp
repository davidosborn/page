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
