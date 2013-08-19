#include <cassert>

#include "ScriptFormat.hpp"

namespace page
{
	namespace res
	{
		std::string GetName(ScriptFormat format)
		{
			switch (format)
			{
				case angelscriptScriptFormat: return "AngelScript"; break;
				case luaScriptFormat:         return "Lua";         break;
				case pythonScriptFormat:      return "Python";      break;
				case squirrelScriptFormat:    return "Squirrel";    break;
				default: assert(!"invalid script format");
			}
		}
	}
}
