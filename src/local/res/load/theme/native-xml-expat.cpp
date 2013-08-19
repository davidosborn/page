#include <cassert>
#include <memory> // {shared,unique}_ptr

#include <expat.h>

#include "../../../pipe/Pipe.hpp" // Pipe::Open
#include "../../../pipe/Stream.hpp" // Stream::~Stream
#include "../../../type/Theme.hpp"
#include "../../LoaderRegistry.hpp" // REGISTER_LOADER

namespace page
{
	namespace res
	{
		Theme *LoadNativeExpatTheme(const std::shared_ptr<const Pipe> &pipe)
		{
			assert(pipe);
			const std::unique_ptr<Stream> stream(pipe->Open());
			// FIXME: implement
			const std::unique_ptr<Theme> theme(new Theme);
			return theme.release();
		}

		LoadFunction GetNativeExpatThemeLoader(const Pipe &pipe)
		{
			const std::unique_ptr<Stream> stream(pipe.Open());
			// FIXME: implement
			return 0;
		}

		REGISTER_LOADER(Theme, GetNativeExpatThemeLoader,
			"theme.xml,xml", "", "", true)
	}
}
