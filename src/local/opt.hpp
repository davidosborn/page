#ifndef    page_local_opt_hpp
#   define page_local_opt_hpp

#	include <string>
#	include <unordered_map>
#	include <vector>

namespace page
{
	namespace opt
	{
		typedef std::vector<std::string> CfgSources;
		extern CfgSources cfgSources;
		typedef std::unordered_map<std::string, std::string> CfgVars;
		extern CfgVars cfgVars;
		typedef std::vector<std::string> ResSources;
		extern ResSources resSources;

		void Parse(int argc, char *argv[]);
	}
}

#endif
