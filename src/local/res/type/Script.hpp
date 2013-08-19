#ifndef    page_local_res_type_Script_hpp
#   define page_local_res_type_Script_hpp

#	include <string>

#	include "script/ScriptFormat.hpp"

namespace page
{
	namespace res
	{
		struct Script
		{
			typedef ScriptFormat Format;
			Format format;
			std::string text;
		};
	}
}

#endif
