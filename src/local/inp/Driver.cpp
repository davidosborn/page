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

#include <cassert>
#include <cmath> // abs
#include <functional> // bind
#include <iomanip> // setw
#include <iostream> // cout
#include <string>

#include <boost/io/ios_state.hpp> // ios_all_saver

#include "../math/Aabb.hpp"
#include "../math/float.hpp" // Near
#include "../wnd/Window.hpp"
#include "Device.hpp" // Device::Update
#include "device/registry.hpp" // MakeDevices
#include "Driver.hpp"

namespace page
{
	namespace inp
	{
		namespace
		{
			// printable representations
			std::string Repr(Button button)
			{
				switch (button)
				{
					case leftButton:   return "left";
					case middleButton: return "middle";
					case rightButton:  return "right";
				}
				assert(!"invalid mouse button");
			}
			std::string Repr(Key key)
			{
				switch (key)
				{
					case backspaceKey: return "backspace";
					case deleteKey:    return "delete";
					case downKey:      return "down";
					case enterKey:     return "enter";
					case escapeKey:    return "escape";
					case leftKey:      return "left";
					case pauseKey:     return "pause";
					case printKey:     return "print";
					case recordKey:    return "record";
					case rightKey:     return "right";
					case tabKey:       return "tab";
					case upKey:        return "up";
				}
				assert(!"invalid key");
			}
			std::string Repr(char c)
			{
				if (c >= 0x20 && c <= 0x7e) return std::string(1, c);
				else switch (c)
				{
					case 0x00: return "null";
					case 0x01: return "start of header";
					case 0x02: return "start of text";
					case 0x03: return "end of text";
					case 0x04: return "end of transmission";
					case 0x05: return "enquiry";
					case 0x06: return "acknowledgement";
					case 0x07: return "bell";
					case 0x08: return "backspace";
					case 0x09: return "tab";
					case 0x0a: return "line feed";
					case 0x0b: return "vertical tab";
					case 0x0c: return "form feed";
					case 0x0d: return "carriage return";
					case 0x0e: return "shift out";
					case 0x0f: return "shift in";
					case 0x10: return "data link escape";
					case 0x11: return "device control 1; XON";
					case 0x12: return "device control 2";
					case 0x13: return "device control 3; XOFF";
					case 0x14: return "device control 4";
					case 0x15: return "negative acknowledgement";
					case 0x16: return "synchronous idle";
					case 0x17: return "end of transmission block";
					case 0x18: return "cancel";
					case 0x19: return "end of medium";
					case 0x1a: return "substitute";
					case 0x1b: return "escape";
					case 0x1c: return "file separator";
					case 0x1d: return "group separator";
					case 0x1e: return "record separator";
					case 0x1f: return "unit separator";
					case 0x7f: return "delete";
				}
				return std::string();
			}
		}

		// construct/destroy
		Driver::Driver(wnd::Window &wnd) : wnd(wnd), interface(0),
			cursorMode(pointCursorMode)
		{
			// connect signals
			{
				using std::bind;
				using namespace std::placeholders;
				// connect cursor signals
				downSig.Connect(bind(&Driver::OnDown, this, _1, _2, _3));
				clickSig.Connect(bind(&Driver::OnClick, this, _1, _2, _3));
				dragSig.Connect(bind(&Driver::OnDrag, this, _1, _2, _3));
				dropSig.Connect(bind(&Driver::OnDrop, this, _1, _2, _3, _4));
				cancelDragSig.Connect(bind(&Driver::OnCancelDrag, this, _1, _2, _3, _4));
				scrollSig.Connect(bind(&Driver::OnScroll, this, _1, _2));
				// connect key signals
				charSig.Connect(bind(&Driver::OnChar, this, _1));
				keySig.Connect(bind(&Driver::OnKey, this, _1));
			}
			// initialize devices
			devices = MakeDevices(wnd);
		}
		Driver::~Driver() {}

		// cursor mode
		Driver::CursorMode Driver::GetCursorMode() const
		{
			return cursorMode;
		}
		void Driver::SetCursorMode(CursorMode mode)
		{
			if (mode != cursorMode)
			{
				// NOTE: must call implementation function before changing mode
				// to allow access to GetCursorPosition
				DoSetCursorMode(mode);
				cursorMode = mode;
			}
		}

		// cursor pointing state
		const math::Vector<2> &Driver::GetCursorPosition() const
		{
			assert(cursorMode == pointCursorMode);
			return pointCursor.position;
		}

		// cursor dragging state
		bool Driver::IsDragging() const
		{
			assert(cursorMode == pointCursorMode);
			return pointCursor.dragging;
		}
		const math::Vector<2> &Driver::GetDragOrigin() const
		{
			assert(cursorMode == pointCursorMode && pointCursor.dragging);
			return pointCursor.dragOrigin;
		}

		// control state
		const math::Vector<2> &Driver::GetDirection() const
		{
			return control.direction;
		}
		bool Driver::GetModifier(Modifier modifier) const
		{
			return control.modifiers[modifier];
		}

		// look state
		const math::Euler<> &Driver::GetRotation() const
		{
			return look.rotation;
		}
		float Driver::GetLift() const
		{
			return look.lift;
		}
		float Driver::GetZoom() const
		{
			return look.zoom;
		}

		// update
		void Driver::Update()
		{
			// update cursor
			if (cursorMode == pointCursorMode)
			{
				pointCursor.position = EnterSpace(
					AabbPositionSize(
						wnd.GetPosition(),
						math::Vector<2, int>(wnd.GetSize()) - 1),
					math::Vector<2, int>(GetRawCursorPosition()));
				pointCursor.inRange = Contains(math::Aabb<2>(0, 1), pointCursor.position);
				pointCursor.position = Min(Max(pointCursor.position, 0), 1);
			}
			// update state
			State state(Poll());
			for (Devices::iterator iter(devices.begin()); iter != devices.end(); ++iter)
			{
				Device &device(**iter);
				state += device.Poll();
			}
			Update(state);
		}

		// window access
		wnd::Window &Driver::GetWindow()
		{
			return wnd;
		}
		const wnd::Window &Driver::GetWindow() const
		{
			return wnd;
		}

		// inspiration modifiers
		void Driver::Imbue(const ui::Interface *interface)
		{
			this->interface = interface;
			OnImbue(interface);
		}

		// inspiration access
		const ui::Interface *Driver::GetInterface() const
		{
			return interface;
		}

		// update
		void Driver::Update(const State &state)
		{
			// update control/look state
			control = state.control;
			look    = state.look;
			// execute events
			std::for_each(state.keyEvents.begin(), state.keyEvents.end(), keySig);
			std::for_each(state.charEvents.begin(), state.charEvents.end(), charSig);
		}

		// inspiration notification
		void Driver::OnImbue(const ui::Interface *) {}

		// cursor signal handlers
		void Driver::OnDown(const math::Vector<2> &pos, Button btn, bool _double)
		{
			std::cout << Repr(btn) << " mouse ";
			if (_double) std::cout << "double-";
			std::cout << "down at " << pos << std::endl;
		}
		void Driver::OnClick(const math::Vector<2> &pos, Button btn, bool _double)
		{
			std::cout << Repr(btn) << " mouse ";
			if (_double) std::cout << "double-";
			std::cout << "click at " << pos << std::endl;
		}
		void Driver::OnDrag(const math::Vector<2> &origin, Button btn, bool _double)
		{
			std::cout << Repr(btn) << " mouse ";
			if (_double) std::cout << "double-";
			std::cout << "dragging from " << origin << std::endl;
			pointCursor.dragging = true;
			pointCursor.dragOrigin = origin;
		}
		void Driver::OnDrop(const math::Vector<2> &start, const math::Vector<2> &pos, Button btn, bool _double)
		{
			std::cout << Repr(btn) << " mouse ";
			if (_double) std::cout << "double-";
			std::cout << "dropped at " << pos << std::endl;
			pointCursor.dragging = false;
		}
		void Driver::OnCancelDrag(const math::Vector<2> &start, const math::Vector<2> &pos, Button btn, bool _double)
		{
			std::cout << Repr(btn) << " mouse ";
			if (_double) std::cout << "double-";
			std::cout << "drag cancelled at " << pos << std::endl;
			pointCursor.dragging = false;
		}
		void Driver::OnScroll(const math::Vector<2> &pos, float delta)
		{
			std::cout << "mouse scrolled " << (delta > 0 ? "foward" : "backward") << ' ';
			delta = std::abs(delta);
			if (math::Near(delta, 1.f)) std::cout << "once";
			else if (math::Near(delta, 2.f)) std::cout << "twice";
			else std::cout << delta << " times";
			std::cout << " at " << pos << std::endl;
		}

		// key signal handlers
		void Driver::OnKey(Key key)
		{
			std::cout << "received " << Repr(key) << " key" << std::endl;
		}
		void Driver::OnChar(char c)
		{
			// print hexadecimal representation
			boost::io::ios_all_saver iosFormatSaver(std::cout);
			std::cout.setf(std::ios_base::hex, std::ios_base::basefield);
			std::cout.setf(std::ios_base::showbase);
			std::cout.setf(std::ios_base::internal, std::ios_base::adjustfield);
			std::cout.fill('0');
			std::cout << "character " << std::setw(4) << static_cast<unsigned>(c);
			iosFormatSaver.restore();

			// print friendly representation if available
			std::string repr(Repr(c));
			if (!repr.empty()) std::cout << " (" << repr << ')';
			std::cout << " entered" << std::endl;
		}

		// cursor pointing state
		// construct
		Driver::PointCursor::PointCursor() : inRange(false), dragging(false) {}
	}
}
