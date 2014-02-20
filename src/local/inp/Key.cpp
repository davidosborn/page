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

#include <cassert>

#include "Key.hpp"

namespace page { namespace inp
{
	std::string GetName(Key key)
	{
		switch (key)
		{
			case Key::backspace: return "backspace";
			case Key::delete_:   return "delete";
			case Key::down:      return "down";
			case Key::enter:     return "enter";
			case Key::escape:    return "escape";
			case Key::left:      return "left";
			case Key::pause:     return "pause";
			case Key::print:     return "print";
			case Key::record:    return "record";
			case Key::right:     return "right";
			case Key::tab:       return "tab";
			case Key::up:        return "up";
		}
		assert(!"invalid key");
	}

	std::string GetName(char c)
	{
		if (c >= 0x20 && c <= 0x7e)
			return std::string(1, c);
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
