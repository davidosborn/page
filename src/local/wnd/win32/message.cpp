#include <cassert>

#include <windows.h>

#include "../../err/Exception.hpp"
#include "../../util/locale/convert.hpp" // Convert
#include "../message.hpp" // MessageType

namespace page { namespace wnd
{
	void Message(const std::string &s, MessageType type, const std::string &title)
	{
		UINT icon;
		switch (type)
		{
			case infoMessage:    icon = MB_ICONINFORMATION; break;
			case errorMessage:   icon = MB_ICONERROR;       break;
			case warningMessage: icon = MB_ICONWARNING;     break;
			default: assert(!"invalid message type");
		}
		if (!MessageBox(NULL,
			util::Convert<TCHAR>(s).c_str(),
			util::Convert<TCHAR>(title).c_str(),
			icon | MB_OK | MB_TASKMODAL))
				THROW((err::Exception<err::EnvModuleTag, err::Win32PlatformTag>("failed to create message box") <<
					boost::errinfo_api_function("MessageBox")))
	}
}}
