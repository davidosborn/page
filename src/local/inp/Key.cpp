#include <cassert>

#include "Key.hpp"

namespace page { namespace inp
{
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

	std::string GetName(char c)
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
