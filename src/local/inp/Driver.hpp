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
 *
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

#ifndef    page_local_inp_Driver_hpp
#   define page_local_inp_Driver_hpp

#	include <memory> // shared_ptr
#	include <vector>
#	include "../math/fwd.hpp" // Euler
#	include "../math/Vector.hpp"
#	include "../util/NonCopyable.hpp"
#	include "../util/Signal.hpp"
#	include "Button.hpp"
#	include "Key.hpp"
#	include "State.hpp"

namespace page
{
	namespace env { class Window; }
	namespace ui { class Interface; }

	namespace inp
	{
		class Device;

		struct Driver : util::NonCopyable
		{
			// construct/destroy
			explicit Driver(env::Window &);
			virtual ~Driver();

			// cursor mode
			enum CursorMode
			{
				noCursorMode,
				pointCursorMode,
				lookCursorMode
			};
			CursorMode GetCursorMode() const;
			void SetCursorMode(CursorMode);

			// cursor pointing state
			const math::Vector<2> &GetCursorPosition() const;

			// cursor dragging state
			bool IsDragging() const;
			const math::Vector<2> &GetDragOrigin() const;

			// control state
			const math::Vector<2> &GetDirection() const;
			enum Modifier
			{
				runModifier,
				altModifier
			};
			bool GetModifier(Modifier) const;

			// look state
			const math::Euler<> &GetRotation() const;
			float GetLift() const;
			float GetZoom() const;

			// update
			void Update();

			// window access
			env::Window &GetWindow();
			const env::Window &GetWindow() const;

			// inspiration modifiers
			void Imbue(const ui::Interface *);

			// pointer signals
			util::Signal<void (const math::Vector<2> &position, Button, bool _double)> downSig;
			util::Signal<void (const math::Vector<2> &position, Button, bool _double)> clickSig;
			util::Signal<void (const math::Vector<2> &origin, Button, bool _double)> dragSig;
			util::Signal<void (const math::Vector<2> &origin, const math::Vector<2> &position, Button, bool _double)> dropSig;
			util::Signal<void (const math::Vector<2> &origin, const math::Vector<2> &position, Button, bool _double)> cancelDragSig;
			util::Signal<void (const math::Vector<2> &position, float scroll)> scrollSig;

			// key signals
			util::Signal<void (Key)> keySig;
			util::Signal<void (char)> charSig;

			protected:
			// inspiration access
			const ui::Interface *GetInterface() const;

			private:
			// update
			void Update(const State &);

			// state query
			virtual State Poll() const = 0;

			// cursor mode modifiers
			virtual void DoSetCursorMode(CursorMode) = 0;

			// system cursor state
			virtual math::Vector<2, unsigned> GetRawCursorPosition() const = 0;

			// inspiration notification
			virtual void OnImbue(const ui::Interface *);

			// cursor signal handlers
			void OnDown(const math::Vector<2> &, Button, bool);
			void OnClick(const math::Vector<2> &, Button, bool);
			void OnDrag(const math::Vector<2> &, Button, bool);
			void OnDrop(const math::Vector<2> &, const math::Vector<2> &, Button, bool);
			void OnCancelDrag(const math::Vector<2> &, const math::Vector<2> &, Button, bool);
			void OnScroll(const math::Vector<2> &, float);

			// key signal handlers
			void OnKey(Key);
			void OnChar(char);

			env::Window &wnd;
			const ui::Interface *interface;

			// devices
			typedef std::vector<std::shared_ptr<Device>> Devices;
			Devices devices;

			// cursor mode
			CursorMode cursorMode;

			// cursor pointing state
			struct PointCursor
			{
				// construct
				PointCursor();

				// pointing
				bool inRange;
				math::Vector<2> position;
				// dragging
				bool dragging;
				math::Vector<2> dragOrigin;
			} pointCursor;

			// control/look state
			State::Control control;
			State::Look look;
		};

		// factory function
		Driver *MakeDriver(env::Window &);
	}
}

#endif
