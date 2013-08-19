#include <cassert>

#include <X11/Xlib.h>

#include "../../err/Exception.hpp"
#include "../message.hpp" // MessageType

namespace page { namespace wnd
{
	void Message(const std::string &s, MessageType type, const std::string &title)
	{
		// FIXME: set icon depending on message type
		// FIXME: implement; see
		// http://www.faqs.org/faqs/x-faq/part7/section-8.html
	}
}}
