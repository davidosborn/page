/**
 * @section license
 *
 * Copyright (c) 2006-2012 David Osborn
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

#include <fstream>
#include <iostream> // cout
#include <utility> // tie

#include "../err/Exception.hpp"
#include "../sys/file.hpp" // AbsPath, NormPath
#include "save/Registry.hpp"
#include "type/Registry.hpp"

namespace page
{
	namespace res
	{
		void Save(const void *res, const std::type_info &id, const std::string &path, const std::string &fmt)
		{
			SaveFunction save; std::string saverPath;
			std::tie(save, saverPath) = GLOBAL(save::Registry).GetSaver(id, path, fmt);
			std::string
				normPath(sys::NormPath(saverPath)),
				absPath(sys::AbsPath(normPath));
			std::ofstream fs(absPath.c_str(), std::ios_base::binary | std::ios_base::out);
			if (!fs)
				THROW((err::Exception<err::ResModuleTag, err::FileAccessTag>() <<
					boost::errinfo_file_name(normPath)))
			CallWithPointer(save, res, fs);
			std::cout << GLOBAL(type::Registry).Query(id).name << " saved as " << normPath << std::endl;
		}
	}
}
