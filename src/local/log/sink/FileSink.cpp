#include "../../cfg/vars.hpp"
#include "../../err/Exception.hpp"
#include "../../util/path/filesystem.hpp" // AbsolutePath
#include "FileSink.hpp"

namespace page
{
	namespace log
	{
		/*-------------+
		| constructors |
		+-------------*/

		FileSink::FileSink(std::string path) :
			fs(util::AbsolutePath(path))
		{
			if (!fs)
				THROW((err::Exception<err::LogModuleTag, err::FileTag>("failed to open file stream") <<
					boost::errinfo_api_function("std::ofstream::open") <<
					boost::errinfo_file_name(path) <<
					boost::errinfo_file_open_mode("w")))
		}

		/*----------------------+
		| Stream implementation |
		+----------------------*/

		void FileSink::DoWrite(const std::string &s)
		{
			if (!fs.write(s.data(), s.size()))
				THROW((err::Exception<err::LogModuleTag, err::FileWriteTag>()))
		}
	}
}
