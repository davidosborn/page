#include <iostream> // c{err,out}

#include "../../cfg/vars.hpp"
#include "../../err/Exception.hpp"
#include "StderrSink.hpp"

namespace page
{
	namespace log
	{
		/*--------------------------+
		| constructors & destructor |
		+--------------------------*/

		StderrSink::StderrSink() :
			streambuf(*(*CVAR(logSync) ? std::cout.rdbuf() : std::cerr.rdbuf())) {}

		/*----------------------+
		| Stream implementation |
		+----------------------*/

		void StderrSink::DoWrite(const std::string &s)
		{
			if (streambuf.sputn(s.data(), s.size()) < s.size())
				THROW((err::Exception<err::LogModuleTag, err::StreamWriteTag>() <<
					boost::errinfo_api_function("std::streambuf::sputn")))
		}

		void StderrSink::DoFlush()
		{
			streambuf.pubsync();
		}
	}
}
