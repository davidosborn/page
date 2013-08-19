#include <functional> // mem_fn
#include <iterator> // begin, end
#include <sstream> // ostringstream

#include <boost/range/adaptor/indirected.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/range/iterator_range.hpp>

#include "../../../util/io/separated_ostream_iterator.hpp"

namespace page { namespace cache { namespace opengl
{
	/*-------------+
	| constructors |
	+-------------*/

	template <typename ShaderInputRange>
		ProgramProxy::ProgramProxy(
			ShaderInputRange shaders,
			ENABLE_IF_IMPL(util::is_range<ShaderInputRange>::value)) :
				BasicProxy<vid::opengl::Program>(MakeSignature(shaders)),
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
	
	template <typename ShaderInputRange>
		std::string ProgramProxy::MakeSignature(
			ShaderInputRange shaders,
			ENABLE_IF_IMPL(util::is_range<ShaderInputRange>::value))
	{
		std::ostringstream ss;
		boost::copy(
			boost::adaptors::transform(
				boost::adaptors::indirect(shaders),
				std::mem_fn(&Proxy<res::Mesh>::GetSignature)),
			util::separated_ostream_iterator<std::string>(ss, ','));
		return Signature("OpenGL program", ss.str());
	}
}}}
