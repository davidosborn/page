#include <cassert>
#include <memory> // {shared,unique}_ptr

#include "../../pipe/Pipe.hpp" // Pipe::Open
#include "../../pipe/Stream.hpp" // Stream::GetText
#include "../../type/Script.hpp"
#include "../LoaderRegistry.hpp" // REGISTER_LOADER

namespace page
{
	namespace res
	{
		namespace opengl
		{
			Script *LoadLuaScript(const std::shared_ptr<const Pipe> &pipe)
			{
				assert(pipe);
				const std::unique_ptr<Stream> stream(pipe->Open());
				const std::unique_ptr<Script> script(new Script);
				script->format = luaScriptFormat;
				script->text = stream->GetText();
				return script.release();
			}

			LoadFunction GetLuaScriptLoader(const Pipe &pipe)
			{
				return LoadLuaScript;
			}

			REGISTER_LOADER(Script, GetLuaScriptLoader,
				"lua,luac", "application/x-lua,text/x-lua", "", false)
		}
	}
}
