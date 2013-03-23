/**
 * @section license
 *
 * Copyright (c) 2006-2013 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution, and in the same
 *    place and form as other copyright, license, and disclaimer information.
 *
 * As a special exception, distributions of derivative works in binary form may
 * include an acknowledgement in place of the above copyright notice, this list
 * of conditions, and the following disclaimer in the documentation and/or other
 * materials provided with the distribution, and in the same place and form as
 * other acknowledgements, similar in substance to the following:
 *
 *    Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

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
