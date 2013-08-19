#include <algorithm> // transform
#include <functional> // mem_fn
#include <iterator> // back_inserter, begin, end

namespace page { namespace cache { namespace opengl
{
	/*--------------------------+
	| constructors & destructor |
	+--------------------------*/

	template <typename ShaderInputRange>
		Program::Program(
			ShaderInputRange shaders,
			ENABLE_IF_IMPL((util::is_range<ShaderInputRange>::value))) :
				Program(std::begin(shaders), std::end(shaders)) {}

	template <typename ShaderInputIterator>
		Program::Program(
			ShaderInputIterator firstShader,
			ShaderInputIterator lastShader,
			ENABLE_IF_IMPL((util::is_iterator<ShaderInputIterator>::value)))
	{
		std::transform(firstShader, lastShader, std::back_inserter(shaders),
			std::mem_fn(&Proxy<res::opengl::Shader>::Copy));

		PostInit();
	}
}}}
