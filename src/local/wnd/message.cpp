#include <cassert>

#include "../util/cpp.hpp" // STRINGIZE
#include "msg.hpp"

namespace page
{
	namespace wnd
	{
		void Message(const std::string &s, MessageType type)
		{
			std::string title;
			switch (type)
			{
				case MessageType::info:    title = STRINGIZE(NAME);            break;
				case MessageType::error:   title = STRINGIZE(NAME) " error";   break;
				case MessageType::warning: title = STRINGIZE(NAME) " warning"; break;
				default: assert(!"invalid message type");
			}
			Message(s, type, title);
		}
	}
}
