/**
 * @copyright
 *
 * Copyright (c) 2006-2014 David Osborn
 *
 * Permission is granted to use and redistribute this software in source and
 * binary form, with or without modification, subject to the following
 * conditions:
 *
 * 1. Redistributions in source form must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the same place
 *    and form as other copyright, license, and disclaimer information.
 *
 * 3. Redistributions in binary form must also include an acknowledgement in the
 *    same place and form as other acknowledgements (such as the credits),
 *    similar in substance to the following:
 *
 *       Portions of this software are based on the work of David Osborn.
 *
 * This software is provided "as is", without any express or implied warranty.
 * In no event will the authors be liable for any damages arising out of the use
 * of this software.
 */

#ifndef    page_local_cfg_source_win32_RegistrySource_hpp
#   define page_local_cfg_source_win32_RegistrySource_hpp

#	include <windows.h>

#	include "../Source.hpp"

namespace page { namespace cfg { namespace win32
{
////////// RegistrySource //////////////////////////////////////////////////////

	/**
	 * Represents a folder in the Windows registry.
	 */
	class RegistrySource : public Source
	{
		IMPLEMENT_CLONEABLE(RegistrySource, Source)

		/*-------------+
		| constructors |
		+-------------*/

		public:
		explicit RegistrySource(HKEY key, const std::string &path);

		private:
		/**
		 * Generate a URI from a registry key and path.
		 */
		static std::string MakeUri(HKEY key, const std::string &path);

		/*----------------------+
		| Source implementation |
		+----------------------*/

		private:
		class Reader;
		class Writer;

		public:
		std::unique_ptr<Source::Reader> OpenReader() const override;
		std::unique_ptr<Source::Writer> OpenWriter() const override;

		/*-------------+
		| data members |
		+-------------*/

		private:
		HKEY key;
		const std::string path;
	};
}}}

#endif
