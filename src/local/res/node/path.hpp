#ifndef    page_local_res_node_path_hpp
#   define page_local_res_node_path_hpp

#	include <string>

namespace page
{
	namespace res
	{
		std::string NormPath(const std::string &);
		std::string CatPath(const std::string &, const std::string &);

		std::string BaseName(const std::string &);
		std::string DirName(const std::string &);
	}
}

#endif
