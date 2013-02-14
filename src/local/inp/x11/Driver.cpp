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

#include <climits> // CHAR_BIT
#include <functional> // bind
#include <vector>
#include <X11/Xutil.h> // XLookupString
#include "../../env/x11/Window.hpp"
#include "../../util/algorithm/stdext.hpp" // for_each_if
#include "../../util/functional/locale.hpp" // isprint_function
#include "Driver.hpp"

namespace page
{
	namespace inp
	{
		namespace x11
		{
			const unsigned keyBufferSize = 16;

			// construct
			Driver::Driver(env::x11::Window &wnd) : inp::Driver(wnd)
			{
				using std::bind;
				using namespace std::placeholders;
				eventCon.Reset(wnd.eventSig.Connect(bind(&Driver::OnEvent, this, _1)));
			}

			// window access
			env::x11::Window &Driver::GetWindow()
			{
				return static_cast<env::x11::Window &>(inp::Driver::GetWindow());
			}
			const env::x11::Window &Driver::GetWindow() const
			{
				return static_cast<const env::x11::Window &>(inp::Driver::GetWindow());
			}

			// state query
			State Driver::Poll() const
			{
				// control
				char keys[32];
				XQueryKeymap(GetWindow().GetDisplay(), keys);
				State state;
				state.control.direction = math::Vector<2>(
					(IsKeyDown(XK_Right, keys) || IsKeyDown(XK_KP_Right, keys)) -
					(IsKeyDown(XK_Left,  keys) || IsKeyDown(XK_KP_Left,  keys)),
					(IsKeyDown(XK_Up,    keys) || IsKeyDown(XK_KP_Up,    keys)) -
					(IsKeyDown(XK_Down,  keys) || IsKeyDown(XK_KP_Down,  keys)));
				state.control.modifiers[runModifier] =
					IsKeyDown(XK_Shift_L,    keys) ||
					IsKeyDown(XK_Shift_R,    keys) ||
					IsKeyDown(XK_Shift_Lock, keys);
				state.control.modifiers[altModifier] =
					IsKeyDown(XK_Control_L, keys) ||
					IsKeyDown(XK_Control_R, keys);
				// look
				// FIXME: implement
				// events
				// FIXME: we should be putting key/char events here, rather than
				// firing them off directly from OnEvent
				return state;
			}

			// cursor mode modifiers
			void Driver::DoSetCursorMode(CursorMode mode)
			{
				// FIXME: implement
			}

			// system cursor state
			math::Vector<2, unsigned> Driver::GetRawCursorPosition() const
			{
				Window root, child;
				math::Vector<2, int> rootPosition, winPosition;
				unsigned mask;
				XQueryPointer(
					GetWindow().GetDisplay(),
					GetWindow().GetWindow(),
					&root, &child,
					&rootPosition.x, &rootPosition.y,
					&winPosition.x,  &winPosition.y,
					&mask);
				return rootPosition;
			}

			// window signal handlers
			void Driver::OnEvent(const XEvent &event)
			{
				switch (event.type)
				{
					// keyboard events
					case KeyPress:
					{
						// extract characters and keys from event
						std::vector<char> buffer(keyBufferSize);
						KeySym keysym;
						buffer.resize(XLookupString(
							// HACK: XLookupString requires non-const XKeyEvent
							const_cast<XKeyEvent *>(&event.xkey),
							&*buffer.begin(), buffer.size(), &keysym, NULL));
						// signal characters
						util::for_each_if(buffer.begin(), buffer.end(),
							charSig, util::isprint_function());
						// signal key
						Key key;
						switch (keysym)
						{
							case XK_BackSpace: key = backspaceKey; break;
							case XK_Delete:    key = deleteKey;    break;
							case XK_Down:
							case XK_KP_Down:   key = downKey;      break;
							case XK_Return:
							case XK_KP_Enter:  key = enterKey;     break;
							case XK_Escape:    key = escapeKey;    break;
							case XK_Left:
							case XK_KP_Left:   key = leftKey;      break;
							case XK_Pause:     key = pauseKey;     break;
							case XK_Print:
							case XK_F12:       key = printKey;     break;
							case XK_F9:        key = recordKey;    break;
							case XK_Right:
							case XK_KP_Right:  key = rightKey;     break;
							case XK_Tab:       key = tabKey;       break;
							case XK_Up:
							case XK_KP_Up:     key = upKey;        break;
							default: goto NoKey;
						}
						keySig(key);
						NoKey:;
					}
					break;
				}
			}

			// key state
			bool Driver::IsKeyDown(KeySym keysym, const char keys[32]) const
			{
				KeyCode keycode = XKeysymToKeycode(GetWindow().GetDisplay(), keysym);
				return keys[keycode / CHAR_BIT] & 1 << keycode % CHAR_BIT;
			}
		}

		// factory function
		Driver *MakeDriver(env::Window &wnd)
		{
			return new x11::Driver(dynamic_cast<env::x11::Window &>(wnd));
		}
	}
}
