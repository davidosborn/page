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

#include <cstdlib> // abort
#include <exception> // set_terminate, set_unexpected
#include <iostream> // cerr

#include "../log/manip.hpp" // Clear
#include "../util/gcc/init_priority.hpp" // LOG_INIT_PRIORITY
#include "../wnd/message.hpp" // Message

namespace page
{
	namespace err
	{
		namespace
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
		}
	}
}
