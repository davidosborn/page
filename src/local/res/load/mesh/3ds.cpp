#include <cassert>
#include <memory> // shared_ptr

#include "../../pipe/Pipe.hpp" // Pipe::Open
#include "../../pipe/Stream.hpp"
#include "../../type/Mesh.hpp"
#include "../LoaderRegistry.hpp" // REGISTER_LOADER

namespace page
{
	namespace res
	{
		namespace
		{
		}

		Mesh *Load3dsMesh(const std::shared_ptr<const Pipe> &pipe)
		{
			assert(pipe);
			// FIXME: implement
			Mesh *mesh = new Mesh;
			return mesh;
		}

		LoadFunction Get3dsMeshLoader(const Pipe &pipe)
		{
			return 0;
		}

		REGISTER_LOADER(Mesh, Get3dsMeshLoader, "3ds", "", "", true)
	}
}
