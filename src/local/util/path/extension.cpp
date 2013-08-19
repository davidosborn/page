#include <string>
#include <utility> // pair

namespace page
{
	namespace util
	{
		std::string GetExtension(const std::string &path, unsigned index)
		{
			std::string::size_type start, end(path.npos);
			for (;;)
			{
				start = path.rfind('.', end);
				if (!index || start == path.npos) break;
				end = start;
				--index;
			}
			return start != path.npos ? path.substr(start + 1, end) : "";
		}

		std::pair<std::string, std::string>
			PartitionExtension(const std::string &path)
		{
			auto i(path.rfind('.'));
			return i != path.npos ?
				{path.substr(0, i), path.substr(i + 1)} :
				{path, ""};
		}

		std::string AddExtension(
			std::string        path,
			std::string const& extension)
		{
			if (path.empty() || path.back() != '.')
				path += '.';
			return path += extension;
		}
	}
}
