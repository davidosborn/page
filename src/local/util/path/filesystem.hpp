#ifndef    page_local_util_path_filesystem_hpp
#   define page_local_util_path_filesystem_hpp

#	include <string>

#	include "../../cfg/vars.hpp"

namespace page
{
	namespace util
	{
		/**
		 * Returns the path in absolute form, relative to the specified root or
		 * the installation directory by default.
		 */
		std::string AbsolutePath(
			std::string const& path,
			std::string const& root = *CVAR(installPath));

		/**
		 * Returns the most user-friendly representation of the path.
		 */
		std::string PrettyPath(const std::string &path);

		/**
		 * Returns the path as a URL, relative to the specified root or the
		 * installation directory by default.
		 */
		std::string UrlPath(
			std::string const& path,
			std::string const& root = *CVAR(installPath));

		/**
		 * Returns the path in relative form, relative to the specified root or
		 * the installation directory by default.
		 */
		std::string RelativePath(
			std::string const& path,
			std::string const& root = *CVAR(installPath));
	}
}

#endif
