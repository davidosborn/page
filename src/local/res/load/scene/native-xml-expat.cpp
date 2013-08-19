#include <cassert>
#include <memory> // {shared,unique}_ptr

#include <expat.h>

#include "../../../pipe/Pipe.hpp" // Pipe::Open
#include "../../../pipe/Stream.hpp" // Stream::~Stream
#include "../../../type/Scene.hpp"
#include "../../LoaderRegistry.hpp" // REGISTER_LOADER

namespace page
{
	namespace res
	{
		Scene *LoadNativeExpatScene(const std::shared_ptr<const Pipe> &pipe)
		{
			assert(pipe);
			const std::unique_ptr<Stream> stream(pipe->Open());
			// FIXME: implement
			const std::unique_ptr<Scene> scene(new Scene);
			return scene.release();
		}

		LoadFunction GetNativeExpatSceneLoader(const Pipe &pipe)
		{
			const std::unique_ptr<Stream> stream(pipe.Open());
			// FIXME: implement
			return 0;
		}

		REGISTER_LOADER(Scene, GetNativeExpatSceneLoader,
			"scene.xml,xml", "", "", true)
	}
}
