#ifndef    page_local_res_source_ZipSource_hpp
#   define page_local_res_source_ZipSource_hpp

#	include "Source.hpp"

namespace page { namespace res
{
	struct ZipSource : Source
	{
		// construct
		explicit ZipSource(const std::string &);

		static bool CheckPath(const std::string &path);

		// modifiers
		void Refresh();

		private:
		// indexing
		void Index();
		void IndexFile(const std::string &, int);

		std::string path;
		unsigned mtime;
	};
}}

#endif
