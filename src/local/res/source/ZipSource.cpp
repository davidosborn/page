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
