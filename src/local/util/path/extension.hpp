#ifndef    page_local_util_path_extension_hpp
#   define page_local_util_path_extension_hpp

#	include <string>
#	include <utility> // pair

namespace page
{
	namespace util
	{
		/**
		 * Returns the path's extension, without the leading dot character.  If
		 * the path has multiple extensions, the one at the specified index,
		 * counting back from the right, is returned.
		 */
		std::string GetExtension(const std::string &path, unsigned index = 0);

		/**
		 * Returns the two path segments resulting from splitting the path at
		 * the last dot character.
		 */
		std::pair<std::string, std::string>
			PartitionExtension(const std::string &path);

		/**
		 * Adds the extension to the end of the path.  If the path does not end
		 * with a dot character, one is inserted before the extension.
		 */
		std::string AddExtension(
			std::string        path,
			std::string const& extension);
	}
}

#endif
