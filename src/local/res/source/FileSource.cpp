#include "../../sys/file.hpp" // {Abs,Norm}Path, IsFile, ModTime
#include "../pipe/FilePipe.hpp" // FilePipe::{{,~}FilePipe}
#include "FileSource.hpp"
#include "register.hpp" // REGISTER_SOURCE

namespace page { namespace res
{
	FileSource::FileSource(const std::string &path) : path(path)
	{
		Index();
	}

	bool FileSource::CheckPath(const std::string &path)
	{
		std::string absPath(sys::AbsPath(sys::NormPath(path)));
		return sys::IsFile(absPath);
	}

	void FileSource::Refresh()
	{
		if (sys::ModTime(path) != mtime)
		{
			Clear();
			Index();
		}
	}

	void FileSource::Index()
	{
		Source::Index(Node(std::shared_ptr<Pipe>(new FilePipe(path))));
		mtime = sys::ModTime(path);
	}

	REGISTER_SOURCE(FileSource)
}}
