#include <algorithm> // replace
#include <string>

namespace page
{
	namespace res
	{
		std::string NormPath(const std::string &path)
		{
			std::string normPath(path);
			// replace backslashes with forward slashes
			std::replace(normPath.begin(), normPath.end(), '\\', '/');
			// remove leading and trailing slashes
			normPath.erase(0, normPath.find_first_not_of('/'));
			normPath.erase(normPath.find_last_not_of('/') + 1);
			// merge adjacent slashes
			for (std::string::size_type sep(std::string::npos);
				(sep = normPath.find("//", sep + 2)) != std::string::npos;)
				normPath.erase(sep + 1, normPath.find_first_not_of('/', sep + 2) - sep - 1);
			return normPath;
		}
		std::string CatPath(const std::string &path1, const std::string &path2)
		{
			if (path1.empty()) return path2;
			if (path2.empty()) return path1;
			return path1 + '/' + path2;
		}

		std::string BaseName(const std::string &path)
		{
			return path.substr(path.rfind('/') + 1);
		}
		std::string DirName(const std::string &path)
		{
			std::string::size_type sep = path.rfind('/');
			return sep != std::string::npos ? path.substr(0, sep) : "";
		}
	}
}
