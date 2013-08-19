#include <algorithm> // find, sort
#include <functional> // bind, mem_fn

#include "../../../util/functional/pointer.hpp" // dereference
#include "../../../util/io/serialize.hpp"
#include "../../../util/iterator/call_iterator.hpp"
#include "../../../util/iterator/indirect_iterator.hpp"
#include "../../../vid/opengl/Program.hpp"
#include "Program.hpp"

namespace page { namespace cache { namespace opengl
{
	/*----------+
	| observers |
	+----------*/

	std::string Program::GetType() const
	{
		return "program";
	}

	std::string Program::GetSource() const
	{
		std::ostringstream ss;
		std::copy(
			util::make_call_iterator(util::make_indirect_iterator(shaders.begin()),
				std::mem_fn(&Proxy<res::opengl::Shader>::GetSource)),
			util::make_call_iterator(util::make_indirect_iterator(shaders.end()),
				std::mem_fn(&Proxy<res::opengl::Shader>::GetSource)),
			util::separated_ostream_iterator<std::string>(ss, ','));
	}

	Program::operator bool() const
	{
		return std::find(
			util::make_indirect_iterator(shaders.begin()),
			util::make_indirect_iterator(shaders.end()),
			false).base() == shaders.end();
	}

	/*--------------+
	| instantiation |
	+--------------*/

	Program::Instance Program::Make() const
	{
		return Instance(new vid::opengl::Program(
			util::make_indirect_iterator(util::make_indirect_iterator(shaders.begin())),
			util::make_indirect_iterator(util::make_indirect_iterator(shaders.end()))));
	}

	/*---------------+
	| initialization |
	+---------------*/

	void Program::PostInit()
	{
		// ensure pointers are valid
		assert(std::find(shaders.begin(), shaders.end(), nullptr) == shaders.end());

		// sort shaders by source
		using namespace std::placeholders;
		sort(shaders.begin(), shaders.end(),
			bind(Proxy<res::opengl::Shader>::CompareSource(),
				bind(util::dereference<util::copy_ptr<Proxy<res::opengl::Shader>>>(), _1),
				bind(util::dereference<util::copy_ptr<Proxy<res::opengl::Shader>>>(), _2)));
	}
}}}
