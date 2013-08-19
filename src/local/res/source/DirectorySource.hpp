#ifndef    page_local_res_source_DirectorySource_hpp
#   define page_local_res_source_DirectorySource_hpp

#	include <unordered_map>

#	include "Source.hpp"

namespace page { namespace res
{
	struct DirectorySource : Source
	{
		// construct
		explicit DirectorySource(const std::string &path);

		static bool CheckPath(const std::string &path);

		// modifiers
		void Refresh();

		private:
		// indexing
		void IndexFile(const std::string &path);

		std::string path;
		struct File
		{
			std::string absPath;
			unsigned mtime;
		};
		typedef std::unordered_map<std::string, File> Files;
		Files files;
	};
}}

#endif
