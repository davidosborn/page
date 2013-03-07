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

#include <cstring> // memcmp
#include <memory> // unique_ptr

#include <minizip/unzip.h>

#include "../../util/cpp.hpp" // STRINGIZE
#include "../adapt/minizip.hpp" // MakeZlibFileFuncDef
#include "../fmt/zip.hpp" // sig
#include "../node/Node.hpp"
#include "../pipe/MinizipPipe.hpp"
#include "../pipe/Pipe.hpp" // Pipe::Open
#include "../pipe/Stream.hpp"
#include "Registry.hpp" // REGISTER_SCANNER

namespace page
{
	namespace res
	{
		bool ScanMinizip(const std::shared_ptr<const Pipe> &pipe, const ScanCallback &cb)
		{
			// check signature; minizip throws if not enough data is available
			// to contain the header, whether or not the signature is correct
			{
				const std::unique_ptr<Stream> stream(pipe->Open());
				char sig[sizeof fmt::sig];
				if (stream->ReadSome(sig, sizeof sig) != sizeof sig ||
					std::memcmp(sig, fmt::sig, sizeof sig)) return false;
			}
			zlib_filefunc_def zlibFileFuncDef(MakeZlibFileFuncDef(*pipe));
			// FIXME: if pipe knows its path, pass it here
			unzFile file = unzOpen2("", &zlibFileFuncDef);
			if (!file) return false;
			try
			{
				for (int result = unzGoToFirstFile(file); result == UNZ_OK; result = unzGoToNextFile(file))
				{
					unz_file_pos pos;
					unzGetFilePos(file, &pos);
					unz_file_info info;
					unzGetCurrentFileInfo(file, &info, 0, 0, 0, 0, 0, 0);
					std::string path(info.size_filename, 0);
					unzGetCurrentFileInfo(file, 0, &*path.begin(), path.size(), 0, 0, 0, 0);
					cb(Node(std::shared_ptr<Pipe>(new MinizipPipe(pipe, pos)), path));
				}
			}
			catch (...)
			{
				unzClose(file);
				throw;
			}
			unzClose(file);
			return true;
		}

		REGISTER_SCANNER(
			STRINGIZE(MINIZIP_NAME),
			ScanMinizip,
			{"pk2", "pk3", "pk4", "zip"},
			{"application/octet-stream", "application/x-zip", "application/zip"})
	}
}
