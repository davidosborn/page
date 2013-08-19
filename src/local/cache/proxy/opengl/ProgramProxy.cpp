#include <algorithm> // find, sort
#include <cassert>
#include <functional> // bind, mem_fn

#include "../../../util/iterator/indirect_iterator.hpp"
#include "../../../vid/opengl/Program.hpp"
#include "Program.hpp"

namespace page { namespace cache { namespace opengl
{
	/*-------------+
	| constructors |
	+-------------*/

	void ProgramProxy::Init()
	{
		// ensure dependencies are valid
		assert(std::find(shaders.begin(), shaders.end(), nullptr) == shaders.end());

		// sort shaders by signature
		sort(shaders.begin(), shaders.end());
	}

	/*--------------------------+
	| BasicProxy implementation |
	+--------------------------*/

	auto ProgramProxy::DoLock() const -> pointer
	{
		return pointer(new vid::opengl::Program(
			util::make_indirect_iterator(util::make_indirect_iterator(shaders.begin())),
			util::make_indirect_iterator(util::make_indirect_iterator(shaders.end()))));
	}
}}}
