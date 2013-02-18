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

#include <cassert>

// Win32
#include <windows.h>

// local
#include "../../err/Exception.hpp"
#include "../../util/win32/string.hpp" // Native
#include "../msg.hpp" // MessageType

namespace page
{
	namespace wnd
	{
		void Message(const std::string &msg, MessageType type, const std::string &title)
		{
			UINT icon;
			switch (type)
			{
				case infoMessage: icon = MB_ICONINFORMATION; break;
				case errorMessage: icon = MB_ICONERROR; break;
				case warningMessage: icon = MB_ICONWARNING; break;
				default: assert(!"invalid message type");
			}
			if (!MessageBox(NULL, util::win32::Native(msg).c_str(),
				util::win32::Native(title).c_str(), icon | MB_OK | MB_TASKMODAL))
					THROW((err::Exception<err::EnvModuleTag, err::Win32PlatformTag>("failed to create message box") <<
						boost::errinfo_api_function("MessageBox")))
		}
	}
}
