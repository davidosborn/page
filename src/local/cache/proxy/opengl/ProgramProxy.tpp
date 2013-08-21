#include <iterator> // begin, end

#include <boost/range/iterator_range.hpp>

namespace page { namespace cache { namespace opengl
{
	/*-------------+
	| constructors |
	+-------------*/

	template <typename ShaderInputRange>
		ProgramProxy::ProgramProxy(
			ShaderInputRange shaders,
			ENABLE_IF_IMPL(util::is_range<ShaderInputRange>::value)) :
				BasicProxy(Signature("OpenGL program", shaders)),
				shaders(std::begin(shaders), std::end(shaders))
	{
		Init();
	}

	template <typename ShaderInputIterator>
		ProgramProxy::ProgramProxy(
			ShaderInputIterator firstShader,
			ShaderInputIterator lastShader,
			ENABLE_IF_IMPL(util::is_iterator<ShaderInputIterator>::value)) :
				Program(boost::iterator_range(firstShader, lastShader)) {}
}}}
