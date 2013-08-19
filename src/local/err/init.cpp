#include <cstdlib> // abort
#include <exception> // set_terminate, set_unexpected
#include <iostream> // cerr

#include "../log/manip.hpp" // Clear
#include "../util/gcc/init_priority.hpp" // LOG_INIT_PRIORITY
#include "../wnd/message.hpp" // Message

namespace page { namespace err { namespace
{
	void unexpected()
	{
		std::cerr << log::Clear;
		wnd::Message("unexpected exception", wnd::MessageType::error);
		std::abort();
	}

	void terminate()
	{
		std::cerr << log::Clear;
		wnd::Message("abnormal termination", wnd::MessageType::error);
		std::abort();
	}

	struct Initializer
	{
		Initializer()
		{
			// FIXME: disabled until we can spend more time on it
			/*std::set_unexpected(&unexpected);
			std::set_terminate(&terminate);*/
		}

	} initializer __attribute__((init_priority(ERR_INIT_PRIORITY)));
}}}
