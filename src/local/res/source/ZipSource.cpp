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

#include <fstream>

#include <zip.h>

#include "../../sys/file.hpp"
#include "../adapt/zip.hpp" // ZipError
#include "../fmt/zip.hpp" // sig
#include "../node/path.hpp" // NormPath
#include "../pipe/ZipPipe.hpp"
#include "register.hpp" // REGISTER_SOURCE
#include "ZipSource.hpp"

namespace page { namespace res
{
	// construct
	ZipSource::ZipSource(const std::string &path) : path(path)
	{
		Index();
	}

	bool ZipSource::CheckPath(const std::string &path)
	{
		std::string absPath(sys::AbsPath(sys::NormPath(path)));
		// check signature
		std::ifstream fs(absPath, std::ios_base::binary);
		if (!fs) return false;
		char sig[sizeof fmt::sig];
		if (fs.read(sig, sizeof sig).bad() || fs.gcount() != sizeof sig) return false;
		return true;
	}

	// modifiers
	void ZipSource::Refresh()
	{
		if (sys::ModTime(path) != mtime)
		{
			Clear();
			Index();
		}
	}

	// indexing
	void ZipSource::Index()
	{
		int ze = 0;
		zip *archive = zip_open(path.c_str(), 0, &ze);
		if (!archive) ZipError(ze);
		for (int i = 0; i < zip_get_num_files(archive); ++i)
			IndexFile(zip_get_name(archive, i, 0), i);
		if (zip_close(archive) == -1) ZipError(archive);
		mtime = sys::ModTime(path);
	}

	void ZipSource::IndexFile(const std::string &path, int i)
	{
		const std::string resPath(NormPath(path));
		Source::Index(Node(std::shared_ptr<Pipe>(new ZipPipe(this->path, i)), resPath));
	}

	REGISTER_SOURCE(ZipSource, 50)
}}
