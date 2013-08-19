#include <exception>
#include <iostream> // c{err,log}

#include <boost/exception/diagnostic_information.hpp>

#include "../log/Indenter.hpp"
#include "../log/manip.hpp" // Error, Warning
#include "../wnd/message.hpp" // Message

namespace page { namespace err
{
	void ReportError(const std::exception &e)
	{
		std::cerr << log::Error << '\n';
		log::Indenter indenter;
		auto info = boost::diagnostic_information(e);
		std::cerr << info << std::endl;
		wnd::Message(info, wnd::MessageType::error);
	}

	void ReportWarning(const std::exception &e)
	{
		std::clog << log::Warning << '\n';
		log::Indenter indenter;
		std::clog << boost::diagnostic_information(e) << std::endl;
	}
}}
