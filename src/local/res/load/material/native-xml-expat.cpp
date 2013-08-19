#include <cassert>
#include <memory> // {shared,unique}_ptr

#include <expat.h>

#include "../../../pipe/Pipe.hpp" // Pipe::Open
#include "../../../pipe/Stream.hpp" // Stream::~Stream
#include "../../../type/Material.hpp"
#include "../../LoaderRegistry.hpp" // REGISTER_LOADER

namespace page
{
	namespace res
	{
		Material *LoadNativeExpatMaterial(const std::shared_ptr<const Pipe> &pipe)
		{
			assert(pipe);
			const std::unique_ptr<Stream> stream(pipe->Open());
			// FIXME: implement
			const std::unique_ptr<Material> mat(new Material);
			return mat.release();
		}

		LoadFunction GetNativeExpatMaterialLoader(const Pipe &pipe)
		{
			const std::unique_ptr<Stream> stream(pipe.Open());
			// FIXME: implement
			return 0;
		}

		REGISTER_LOADER(Material, GetNativeExpatMaterialLoader,
			"mat.xml,material.xml,xml", "", "", true)
	}
}
