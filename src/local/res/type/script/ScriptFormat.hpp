#ifndef    page_local_res_type_script_ScriptFormat_hpp
#   define page_local_res_type_script_ScriptFormat_hpp

#	include <string>

namespace page
{
	namespace res
	{
		enum ScriptFormat
		{
			angelscriptScriptFormat,
			luaScriptFormat,
			pythonScriptFormat,
			squirrelScriptFormat
		};

		std::string GetName(ScriptFormat);
	}
}

#endif
