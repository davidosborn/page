#ifndef    page_local_util_path_expand_hpp
#   define page_local_util_path_expand_hpp

#	include <string>

#	include "../Flags.hpp"

namespace page
{
	namespace util
	{
		/**
		 * A collection of bit-flags for modifying the behaviour of
		 * @c ExpandPath.
		 */
		struct ExpandFlags : Flags
		{
			enum
			{
				/**
				 * Interprets the path as if it had a wildcard suffix.
				 */
				withImplicitWildcardSuffix = 0x01
			};
		};

		/**
		 * Parse and expand the special symbols in a path.
		 *
		 * Supports all of the symbols supported by strftime, as well as the
		 * following extensions:
		 *
		 * %i A number that is incremented each time the file is saved.  The
		 *    function inserts the lowest number that will not conflict with an
		 *    existing filename.  An optional width modifier may be specified
		 *    immediately after the percent sign, which must be an unsigned
		 *    decimal integer.  Multiple incremental symbols form an overflow
		 *    cascade, where the last one increments first.  If no unique
		 *    incremental filename is found, an exception is thrown.
		 *
		 * @note If the path contains wildcards, the function will avoid any
		 *       matching files when processing %i symbols.
		 */
		std::string ExpandPath(const std::string &, int flags = 0);
	}
}

#endif
