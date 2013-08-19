#ifndef    page_local_util_platform_x11_event_hpp
#   define page_local_util_platform_x11_event_hpp

#	include <string>

#	include <X11/X.h> // XEvent

namespace page
{
	namespace util
	{
		namespace x11
		{
			std::string GetEventInfo(const XEvent &);
			std::string GetEventTypeName(int);
		}
	}
}

#endif
