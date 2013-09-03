#include <iostream> // cout

#include "../../err/Exception.hpp"
#include "StdoutSink.hpp"

namespace page
{
	namespace log
	{
		/*-------------+
		| constructors |
		+-------------*/

		StdoutSink::StdoutSink() :
			streambuf(*std::cout.rdbuf()) {}

		/*----------------------+
		| Stream implementation |
		+----------------------*/

		void StdoutSink::DoWrite(const std::string &s)
		{
			if (streambuf.sputn(s.data(), s.size()) < s.size())
				THROW((err::Exception<err::LogModuleTag, err::StreamWriteTag>() <<
					boost::errinfo_api_function("std::streambuf::sputn")))
		}

		void StdoutSink::DoFlush()
		{
			streambuf.pubsync();
		}
	}
}
