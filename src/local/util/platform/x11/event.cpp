#include <string>

#include <X11/Xlib.h>

namespace page
{
	namespace util
	{
		namespace x11
		{
			std::string GetEventInfo(const XEvent &event)
			{
				switch (event.type)
				{
					// FIXME: implement
					default: return "";
				}
			}
			std::string GetEventTypeName(int type)
			{
				switch (type)
				{
					case ButtonPress:      return "button press";
					case ButtonRelease:    return "button release";
					case CirculateNotify:  return "circulate notify";
					case CirculateRequest: return "circulate request";
					case ClientMessage:    return "client message";
					case ColormapNotify:   return "colormap notify";
					case ConfigureNotify:  return "configure notify";
					case ConfigureRequest: return "configure request";
					case CreateNotify:     return "create notify";
					case DestroyNotify:    return "destroy notify";
					case EnterNotify:      return "enter notify";
					case Expose:           return "expose";
					case FocusIn:          return "focus in";
					case FocusOut:         return "focus out";
					case GraphicsExpose:   return "graphics expose";
					case GravityNotify:    return "gravity notify";
					case KeymapNotify:     return "keymap notify";
					case KeyPress:         return "key press";
					case KeyRelease:       return "key release";
					case LeaveNotify:      return "leave notify";
					case MapNotify:        return "map notify";
					case MappingNotify:    return "mapping notify";
					case MapRequest:       return "map request";
					case MotionNotify:     return "motion notify";
					case NoExpose:         return "no expose";
					case PropertyNotify:   return "property notify";
					case ReparentNotify:   return "reparent notify";
					case ResizeRequest:    return "resize request";
					case SelectionClear:   return "selection clear";
					case SelectionNotify:  return "selection notify";
					case SelectionRequest: return "selection request";
					case UnmapNotify:      return "unmap notify";
					case VisibilityNotify: return "visibility notify";
					default:               return "unknown";
				}
			}
		}
	}
}
