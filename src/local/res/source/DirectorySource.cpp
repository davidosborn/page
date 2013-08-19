#include <functional> // bind
#include <regex>

#include "../../err/Exception.hpp"
#include "../../sys/file.hpp" // {Abs,Cat,Norm}Path, IsDir, ModTime, WalkDir
#include "../node/path.hpp" // NormPath
#include "../pipe/FilePipe.hpp" // FilePipe::{,~}FilePipe
#include "DirectorySource.hpp"
#include "register.hpp" // REGISTER_SOURCE

namespace page { namespace res
{
	// construct
	DirectorySource::DirectorySource(const std::string &path) : path(path)
	{
		sys::WalkDir(path, std::bind(&DirectorySource::IndexFile, this, std::placeholders::_1));
	}

	bool DirectorySource::CheckPath(const std::string &path)
	{
		std::string absPath(sys::AbsPath(sys::NormPath(path)));
		return sys::IsDir(absPath);
	}

	// modifiers
	void DirectorySource::Refresh()
	{
		for (Files::iterator fileIter(files.begin()); fileIter != files.end();)
		{
			const std::string &path(fileIter->first);
			File &file(fileIter->second);
			try
			{
				if (sys::ModTime(file.absPath) != file.mtime)
				{
					Clear(path);
					IndexFile(path);
				}
			}
			catch (const err::Exception<err::FileTag>::Permutation &)
			{
				Clear(path);
				files.erase(fileIter++);
				continue;
			}
			++fileIter;
		}
	}

	// indexing
	void DirectorySource::IndexFile(const std::string &path)
	{
		std::string resPath(NormPath(path));
		std::string absPath(sys::CatPath(this->path, path));
		Index(Node(std::shared_ptr<Pipe>(new FilePipe(absPath)), resPath));
		// store file information
		File file = {absPath, sys::ModTime(absPath)};
		files.insert(std::make_pair(resPath, File())).first->second = file;
	}

	REGISTER_SOURCE(DirectorySource)
}}
