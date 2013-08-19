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
			{"application/octet-stream", "application/x-zip", "application/zip"},
			{"pk2", "pk3", "pk4", "zip"})
	}
}
