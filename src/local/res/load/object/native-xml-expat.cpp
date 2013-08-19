#include <cassert>
#include <memory> // {shared,unique}_ptr

#include <expat.h>

#include "../../../pipe/Pipe.hpp" // Pipe::Open
#include "../../../pipe/Stream.hpp" // Stream::~Stream
#include "../../../type/Object.hpp"
#include "../../LoaderRegistry.hpp" // REGISTER_LOADER

namespace page
{
	namespace res
	{
		Object *LoadNativeExpatObject(const std::shared_ptr<const Pipe> &pipe)
		{
			assert(pipe);
			const std::unique_ptr<Stream> stream(pipe->Open());
			// FIXME: implement
			const std::unique_ptr<Object> object(new Object);
			return object.release();
		}

		LoadFunction GetNativeExpatObjectLoader(const Pipe &pipe)
		{
			const std::unique_ptr<Stream> stream(pipe.Open());
			// FIXME: implement
			return 0;
		}

		REGISTER_LOADER(Object, GetNativeExpatObjectLoader,
			"obj.xml,object.xml,xml", "", "", true)
	}
}
