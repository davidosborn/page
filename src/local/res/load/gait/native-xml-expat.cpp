#include <cassert>
#include <memory> // {shared,unique}_ptr

#include <expat.h>

#include "../../../pipe/Pipe.hpp" // Pipe::Open
#include "../../../pipe/Stream.hpp" // Stream::~Stream
#include "../../../type/Gait.hpp"
#include "../../LoaderRegistry.hpp" // REGISTER_LOADER

namespace page
{
	namespace res
	{
		Gait *LoadNativeExpatGait(const std::shared_ptr<const Pipe> &pipe)
		{
			assert(pipe);
			const std::unique_ptr<Stream> stream(pipe->Open());
			// FIXME: implement
			const std::unique_ptr<Gait> gait(new Gait);
			return gait.release();
		}

		LoadFunction GetNativeExpatGaitLoader(const Pipe &pipe)
		{
			const std::unique_ptr<Stream> stream(pipe.Open());
			// FIXME: implement
			return 0;
		}

		REGISTER_LOADER(Gait, GetNativeExpatGaitLoader,
			"gait.xml,xml", "", "", true)
	}
}
