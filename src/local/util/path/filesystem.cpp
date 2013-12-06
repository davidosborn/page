#include <string>

#include <boost/filesystem/path.hpp>

namespace page { namespace util
{
	std::string AbsolutePath(
		std::string const& path,
		std::string const& root)
	{
		// FIXME: implement
	}

	std::string Basename(const std::string &path)
	{
		return path.substr(path.rfind('/') + 1);
	}

	std::string Dirname(const std::string &path)
	{
		auto sep(path.rfind('/'));
		return sep != std::string::npos ? path.substr(0, sep) : "";
	}

	std::string PrettyPath(const std::string &path)
	{
		// FIXME: implement
	}

	std::string RelativePath(
		std::string const& path,
		std::string const& root)
	{
		// FIXME: implement
	}

	std::string UrlPath(
		std::string const& path,
		std::string const& root)
	{
		// FIXME: implement
	}
}}
