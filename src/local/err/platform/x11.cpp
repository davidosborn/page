#include <string>

#include <X11/Xlib.h> // Bad*

namespace page { namespace err { namespace x11
{
	// FIXME: provide error handler using XSetErrorHandler

	std::string GetErrorString(unsigned char e)
	{
		switch (e)
		{
			case BadAccess:         return "access denied";
			case BadAlloc:          return "out of memory";
			case BadAtom:           return "invalid atom";
			case BadColor:          return "invalid color";
			case BadCursor:         return "invalid cursor";
			case BadDrawable:       return "invalid drawable";
			case BadGC:             return "invalid graphics context";
			case BadIDChoice:       return "invalid resource identifier";
			case BadImplementation: return "not implemented";
			case BadLength:         return "request size mismatch";
			case BadMatch:          return "format mismatch";
			case BadName:           return "invalid name";
			case BadPixmap:         return "invalid pixmap";
			case BadRequest:        return "invalid request";
			case BadValue:          return "out of range";
			case BadWindow:         return "invalid window";
			default:                return "unknown error";
		}
	}
}}}
