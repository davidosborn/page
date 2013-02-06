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
